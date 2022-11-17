#pragma once

// stl
#include <iostream>
#include <fstream>
#include <assert.h>

// polyvox
#include <PolyVox/RawVolume.h>
#include <PolyVox/CubicSurfaceExtractor.h>
#include <PolyVox/VolumeResampler.h>

// putils
#include "MagicaVoxel.hpp"
#include "file_extension.hpp"
#include "Color.hpp"
#include "Point.hpp"
#include "putils_profiling.hpp"

namespace putils::MagicaVoxel {
	struct VertexData {
		float color[3] = { 0.f, 0.f, 0.f };

		putils_reflection_attributes(
			putils_reflection_attribute(&VertexData::color)
		);

		bool operator==(size_t i) const {
			return color[0] == 0.f && color[1] == 0.f && color[2] == 0.f;
		}

		bool operator>(size_t i) const {
			return color[0] != 0.f || color[1] != 0.f || color[2] != 0.f;
		}

		bool operator==(const VertexData & rhs) const {
			return color[0] == rhs.color[0] && color[1] == rhs.color[1] && color[2] == rhs.color[2];
		}
	};

	namespace detail {
		static auto buildMesh(PolyVox::RawVolume<VertexData> & volume) noexcept {
			PUTILS_PROFILING_SCOPE;

			const auto encodedMesh = PolyVox::extractCubicMesh(&volume, volume.getEnclosingRegion());
			const auto mesh = PolyVox::decodeMesh(encodedMesh);
			return mesh;
		}

		using MeshType = decltype(buildMesh(PolyVox::RawVolume<VertexData>{ {} }));

		static bool idMatches(const char * s1, const char * s2) noexcept {
			return strncmp(s1, s2, 4) == 0;
		}

		template<typename T>
		static void readFromStream(T & header, std::istream & s, unsigned int size) noexcept {
			PUTILS_PROFILING_SCOPE;

			s.read((char *)&header, size);
			assert(s.gcount() == size);
		}

		template<typename T>
		static void readFromStream(T & header, std::istream & s) noexcept {
			readFromStream(header, s, sizeof(header));
		}

		static void checkHeader(std::istream & s) noexcept {
			PUTILS_PROFILING_SCOPE;

			MagicaVoxel::FileHeader header;
			readFromStream(header, s);
			assert(idMatches(header.id, "VOX "));
			assert(header.versionNumber == 150);
		}
	}

	static auto loadVoxFile(const char * f, putils::Point3i * outSize = nullptr) noexcept {
		PUTILS_PROFILING_SCOPE;

#ifndef KENGINE_NDEBUG
		std::cout << "[MagicaVoxel] Loading " << f << "...";
#endif
		std::ifstream stream(f, std::ios::binary);
		assert(stream);
		detail::checkHeader(stream);

		MagicaVoxel::ChunkHeader main;
		detail::readFromStream(main, stream);
		assert(detail::idMatches(main.id, "MAIN"));

		MagicaVoxel::ChunkHeader first;
		detail::readFromStream(first, stream);
		assert(detail::idMatches(first.id, "SIZE")); // "PACK" not yet supported
		assert(first.childrenBytes == 0);
		assert(first.contentBytes == sizeof(MagicaVoxel::ChunkContent::Size));

		MagicaVoxel::ChunkContent::Size size;
		detail::readFromStream(size, stream);
		if (outSize != nullptr) {
			outSize->x = size.x;
			outSize->y = size.y;
			outSize->z = size.z;
		}

		PolyVox::RawVolume<VertexData> volume(PolyVox::Region{ { 0, 0, 0, }, { size.x, size.z, size.y } });

		MagicaVoxel::ChunkHeader voxelsHeader;
		detail::readFromStream(voxelsHeader, stream);
		assert(detail::idMatches(voxelsHeader.id, "XYZI"));

		MagicaVoxel::ChunkContent::RGBA rgba;

		MagicaVoxel::ChunkContent::XYZI xyzi;
		detail::readFromStream(xyzi, stream);
		assert(voxelsHeader.contentBytes == sizeof(xyzi) + xyzi.numVoxels * sizeof(int));

		for (int i = 0; i < xyzi.numVoxels; ++i) {
			MagicaVoxel::ChunkContent::XYZI::Voxel voxel;
			detail::readFromStream(voxel, stream);

			const auto & color = rgba.palette[voxel.colorIndex];
			const VertexData voxelValue{ { (float)color.r / 255.f, (float)color.g / 255.f, (float)color.b / 255.f } };
			volume.setVoxel(voxel.x, voxel.z, voxel.y, voxelValue);
		}

#ifndef KENGINE_NDEBUG
		std::cout << " Done." << std::endl;
#endif

		return detail::buildMesh(volume);
	}
}