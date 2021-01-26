/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/core/SkVertices.h"

#include "include/core/SkData.h"
#include "include/private/SkTo.h"
#include "src/core/SkReadBuffer.h"
#include "src/core/SkSafeMath.h"
#include "src/core/SkSafeRange.h"
#include "src/core/SkVerticesPriv.h"
#include "src/core/SkWriteBuffer.h"
#include <atomic>
#include <new>

static int32_t next_id() {
    static std::atomic<int32_t> nextID{1};

    int32_t id;
    do {
        id = nextID++;
    } while (id == SK_InvalidGenID);
    return id;
}

int SkVertices::Attribute::channelCount() const {
    switch (fType) {
        case Type::kFloat:       return 1;
        case Type::kFloat2:      return 2;
        case Type::kFloat3:      return 3;
        case Type::kFloat4:      return 4;
        case Type::kByte4_unorm: return 4;
    }
    SkUNREACHABLE;
}

size_t SkVertices::Attribute::bytesPerVertex() const {
    switch (fType) {
        case Type::kFloat:       return 1 * sizeof(float);
        case Type::kFloat2:      return 2 * sizeof(float);
        case Type::kFloat3:      return 3 * sizeof(float);
        case Type::kFloat4:      return 4 * sizeof(float);
        case Type::kByte4_unorm: return 4 * sizeof(uint8_t);
    }
    SkUNREACHABLE;
}

static size_t custom_data_size(const SkVertices::Attribute* attrs, int attrCount) {
    size_t size = 0;
    for (int i = 0; i < attrCount; ++i) {
        size += attrs[i].bytesPerVertex();
    }
    return size;
}

struct SkVertices::Desc {
    VertexMode  fMode;
    int         fVertexCount,
                fIndexCount;
    bool        fHasTexs,
                fHasColors;

    const Attribute* fAttributes;
    int              fAttributeCount;

    void validate() const {
        SkASSERT(fAttributeCount == 0 || (!fHasTexs && !fHasColors));
    }
};

struct SkVertices::Sizes {
    Sizes(const Desc& desc) {
        desc.validate();
        SkSafeMath safe;

        fVSize = safe.mul(desc.fVertexCount, sizeof(SkPoint));
        fDSize = safe.mul(custom_data_size(desc.fAttributes, desc.fAttributeCount),
                          desc.fVertexCount);
        fTSize = desc.fHasTexs ? safe.mul(desc.fVertexCount, sizeof(SkPoint)) : 0;
        fCSize = desc.fHasColors ? safe.mul(desc.fVertexCount, sizeof(SkColor)) : 0;

        fBuilderTriFanISize = 0;
        fISize = safe.mul(desc.fIndexCount, sizeof(uint16_t));
        if (kTriangleFan_VertexMode == desc.fMode) {
            int numFanTris = 0;
            if (desc.fIndexCount) {
                fBuilderTriFanISize = fISize;
                numFanTris = desc.fIndexCount - 2;
            } else {
                numFanTris = desc.fVertexCount - 2;
                // By forcing this to become indexed we are adding a constraint to the maximum
                // number of vertices.
                if (desc.fVertexCount > (SkTo<int>(UINT16_MAX) + 1)) {
                    sk_bzero(this, sizeof(*this));
                    return;
                }
            }
            if (numFanTris <= 0) {
                sk_bzero(this, sizeof(*this));
                return;
            }
            fISize = safe.mul(numFanTris, 3 * sizeof(uint16_t));
        }

        fTotal = safe.add(sizeof(SkVertices),
                 safe.add(fVSize,
                 safe.add(fDSize,
                 safe.add(fTSize,
                 safe.add(fCSize,
                          fISize)))));

        if (safe.ok()) {
            fArrays = fTotal - sizeof(SkVertices);  // just the sum of the arrays
        } else {
            sk_bzero(this, sizeof(*this));
        }
    }

    bool isValid() const { return fTotal != 0; }

    size_t fTotal;  // size of entire SkVertices allocation (obj + arrays)
    size_t fArrays; // size of all the arrays (V + D + T + C + I)
    size_t fVSize;
    size_t fDSize;  // size of all customData = [customDataSize * fVertexCount]
    size_t fTSize;
    size_t fCSize;
    size_t fISize;

    // For indexed tri-fans this is the number of amount of space fo indices needed in the builder
    // before conversion to indexed triangles (or zero if not indexed or not a triangle fan).
    size_t fBuilderTriFanISize;
};

SkVertices::Builder::Builder(VertexMode mode, int vertexCount, int indexCount,
                             uint32_t builderFlags) {
    bool hasTexs = SkToBool(builderFlags & SkVertices::kHasTexCoords_BuilderFlag);
    bool hasColors = SkToBool(builderFlags & SkVertices::kHasColors_BuilderFlag);
    this->init({mode, vertexCount, indexCount, hasTexs, hasColors, nullptr, 0});
}

SkVertices::Builder::Builder(VertexMode mode,
                             int vertexCount,
                             int indexCount,
                             const SkVertices::Attribute* attrs,
                             int attrCount) {
    if (attrCount <= 0 || attrCount > kMaxCustomAttributes || !attrs) {
        return;
    }
    this->init({mode, vertexCount, indexCount, false, false, attrs, attrCount});
}

SkVertices::Builder::Builder(const Desc& desc) {
    this->init(desc);
}

void SkVertices::Builder::init(const Desc& desc) {
    Sizes sizes(desc);
    if (!sizes.isValid()) {
        SkASSERT(!this->isValid());
        return;
    }

    void* storage = ::operator new (sizes.fTotal);
    if (sizes.fBuilderTriFanISize) {
        fIntermediateFanIndices.reset(new uint8_t[sizes.fBuilderTriFanISize]);
    }

    fVertices.reset(new (storage) SkVertices);

    // need to point past the object to store the arrays
    char* ptr = (char*)storage + sizeof(SkVertices);

    // return the original ptr (or null), but then advance it by size
    auto advance = [&ptr](size_t size) {
        char* new_ptr = size ? ptr : nullptr;
        ptr += size;
        return new_ptr;
    };

    fVertices->fPositions     = (SkPoint*) advance(sizes.fVSize);
    fVertices->fCustomData    = (void*)    advance(sizes.fDSize);
    fVertices->fTexs          = (SkPoint*) advance(sizes.fTSize);
    fVertices->fColors        = (SkColor*) advance(sizes.fCSize);
    fVertices->fIndices       = (uint16_t*)advance(sizes.fISize);

    fVertices->fVertexCount   = desc.fVertexCount;
    fVertices->fIndexCount    = desc.fIndexCount;

    sk_careful_memcpy(fVertices->fAttributes, desc.fAttributes,
                      desc.fAttributeCount * sizeof(Attribute));
    fVertices->fAttributeCount = desc.fAttributeCount;

    fVertices->fMode = desc.fMode;

    // We defer assigning fBounds and fUniqueID until detach() is called
}

sk_sp<SkVertices> SkVertices::Builder::detach() {
    if (fVertices) {
        fVertices->fBounds.setBounds(fVertices->fPositions, fVertices->fVertexCount);
        if (fVertices->fMode == kTriangleFan_VertexMode) {
            if (fIntermediateFanIndices.get()) {
                SkASSERT(fVertices->fIndexCount);
                auto tempIndices = this->indices();
                for (int t = 0; t < fVertices->fIndexCount - 2; ++t) {
                    fVertices->fIndices[3 * t + 0] = tempIndices[0];
                    fVertices->fIndices[3 * t + 1] = tempIndices[t + 1];
                    fVertices->fIndices[3 * t + 2] = tempIndices[t + 2];
                }
                fVertices->fIndexCount = 3 * (fVertices->fIndexCount - 2);
            } else {
                SkASSERT(!fVertices->fIndexCount);
                for (int t = 0; t < fVertices->fVertexCount - 2; ++t) {
                    fVertices->fIndices[3 * t + 0] = 0;
                    fVertices->fIndices[3 * t + 1] = SkToU16(t + 1);
                    fVertices->fIndices[3 * t + 2] = SkToU16(t + 2);
                }
                fVertices->fIndexCount = 3 * (fVertices->fVertexCount - 2);
            }
            fVertices->fMode = kTriangles_VertexMode;
        }
        fVertices->fUniqueID = next_id();
        return std::move(fVertices);        // this will null fVertices after the return
    }
    return nullptr;
}

SkPoint* SkVertices::Builder::positions() {
    return fVertices ? const_cast<SkPoint*>(fVertices->fPositions) : nullptr;
}

void* SkVertices::Builder::customData() {
    return fVertices ? const_cast<void*>(fVertices->fCustomData) : nullptr;
}

SkPoint* SkVertices::Builder::texCoords() {
    return fVertices ? const_cast<SkPoint*>(fVertices->fTexs) : nullptr;
}

SkColor* SkVertices::Builder::colors() {
    return fVertices ? const_cast<SkColor*>(fVertices->fColors) : nullptr;
}

uint16_t* SkVertices::Builder::indices() {
    if (!fVertices) {
        return nullptr;
    }
    if (fIntermediateFanIndices) {
        return reinterpret_cast<uint16_t*>(fIntermediateFanIndices.get());
    }
    return const_cast<uint16_t*>(fVertices->fIndices);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

sk_sp<SkVertices> SkVertices::MakeCopy(VertexMode mode, int vertexCount,
                                       const SkPoint pos[], const SkPoint texs[],
                                       const SkColor colors[],
                                       int indexCount, const uint16_t indices[]) {
    auto desc = Desc{mode, vertexCount, indexCount, !!texs, !!colors, nullptr, 0};
    Builder builder(desc);
    if (!builder.isValid()) {
        return nullptr;
    }

    Sizes sizes(desc);
    SkASSERT(sizes.isValid());
    sk_careful_memcpy(builder.positions(), pos, sizes.fVSize);
    sk_careful_memcpy(builder.texCoords(), texs, sizes.fTSize);
    sk_careful_memcpy(builder.colors(), colors, sizes.fCSize);
    size_t isize = (mode == kTriangleFan_VertexMode) ? sizes.fBuilderTriFanISize : sizes.fISize;
    sk_careful_memcpy(builder.indices(), indices, isize);

    return builder.detach();
}

size_t SkVertices::approximateSize() const {
    return sizeof(SkVertices) + this->getSizes().fArrays;
}

SkVertices::Sizes SkVertices::getSizes() const {
    Sizes sizes(
            {fMode, fVertexCount, fIndexCount, !!fTexs, !!fColors, fAttributes, fAttributeCount});
    SkASSERT(sizes.isValid());
    return sizes;
}

size_t SkVerticesPriv::customDataSize() const {
    return custom_data_size(fVertices->fAttributes, fVertices->fAttributeCount);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// storage = packed | vertex_count | index_count | pos[] | texs[] | colors[] | boneIndices[] |
//           boneWeights[] | indices[]
//         = header + arrays

#define kMode_Mask          0x0FF
#define kHasTexs_Mask       0x100
#define kHasColors_Mask     0x200

void SkVertices::encode(SkWriteBuffer& buffer) const{
    // packed has room for addtional flags in the future (e.g. versioning)
    uint32_t packed = static_cast<uint32_t>(fMode);
    SkASSERT((packed & ~kMode_Mask) == 0);  // our mode fits in the mask bits
    if (fTexs) {
        packed |= kHasTexs_Mask;
    }
    if (fColors) {
        packed |= kHasColors_Mask;
    }

    Sizes sizes = this->getSizes();
    SkASSERT(!sizes.fBuilderTriFanISize);

    buffer.writeUInt(packed);
    buffer.writeInt(fVertexCount);
    buffer.writeInt(fIndexCount);
    buffer.writeInt(fAttributeCount);

    for (int i = 0; i < fAttributeCount; ++i) {
        buffer.writeInt(static_cast<int>(fAttributes[i].fType));
    }

    buffer.writeByteArray(fPositions, sizes.fVSize);
    buffer.writeByteArray(fCustomData, sizes.fDSize);
    buffer.writeByteArray(fTexs, sizes.fTSize);
    buffer.writeByteArray(fColors, sizes.fCSize);
    // if index-count is odd, we won't be 4-bytes aligned, so we call the pad version
    buffer.writeByteArray(fIndices, sizes.fISize);
}

sk_sp<SkVertices> SkVertices::Decode(SkReadBuffer& buffer) {
    if (buffer.isVersionLT(SkPicturePriv::kVerticesUseReadBuffer_Version)) {
        // Old versions used an embedded blob that was serialized with SkWriter32/SkReader32.
        // We don't support loading those, but skip over the vertices to keep the buffer valid.
        auto data = buffer.readByteArrayAsData();
        (void)data;
        return nullptr;
    }

    auto decode = [](SkReadBuffer &buffer) -> sk_sp<SkVertices> {
        SkSafeRange safe;
        const uint32_t packed = buffer.readUInt();
        const int vertexCount = safe.checkGE(buffer.readInt(), 0);
        const int indexCount = safe.checkGE(buffer.readInt(), 0);
        const int attrCount = safe.checkGE(buffer.readInt(), 0);
        const VertexMode mode = safe.checkLE<VertexMode>(packed & kMode_Mask,
                                                         SkVertices::kLast_VertexMode);
        const bool hasTexs = SkToBool(packed & kHasTexs_Mask);
        const bool hasColors = SkToBool(packed & kHasColors_Mask);
        // now we finish unpacking the packed field


        if (!safe                                           // Invalid header fields
            || attrCount > kMaxCustomAttributes             // Too many custom attributes?
            || (attrCount > 0 && (hasTexs || hasColors))) { // Overspecified (incompatible features)
            return nullptr;
        }

        SkVertices::Attribute attrs[SkVertices::kMaxCustomAttributes];
        for (int i = 0; i < attrCount; ++i) {
            auto type = buffer.checkRange(SkVertices::Attribute::Type::kFloat,
                                          SkVertices::Attribute::Type::kByte4_unorm);
            attrs[i] = SkVertices::Attribute(type);
        }

        // Ensure that all of the attribute metadata was valid before proceeding
        if (!buffer.isValid()) {
            return nullptr;
        }

        const Desc desc{mode, vertexCount, indexCount, hasTexs, hasColors,
                        attrCount ? attrs : nullptr, attrCount};
        Sizes sizes(desc);
        if (!sizes.isValid()) {
            return nullptr;
        }

        Builder builder(desc);
        if (!builder.isValid()) {
            return nullptr;
        }

        buffer.readByteArray(builder.positions(), sizes.fVSize);
        buffer.readByteArray(builder.customData(), sizes.fDSize);
        buffer.readByteArray(builder.texCoords(), sizes.fTSize);
        buffer.readByteArray(builder.colors(), sizes.fCSize);

        size_t isize = (mode == kTriangleFan_VertexMode) ? sizes.fBuilderTriFanISize : sizes.fISize;
        buffer.readByteArray(builder.indices(), isize);

        if (!buffer.isValid()) {
            return nullptr;
        }
        if (indexCount > 0) {
            // validate that the indices are in range
            const uint16_t* indices = builder.indices();
            for (int i = 0; i < indexCount; ++i) {
                if (indices[i] >= (unsigned)vertexCount) {
                    return nullptr;
                }
            }
        }
        return builder.detach();
    };

    if (auto verts = decode(buffer)) {
        return verts;
    }

    buffer.validate(false);
    return nullptr;
}

void SkVertices::operator delete(void* p) {
    ::operator delete(p);
}
