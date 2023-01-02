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
#include "magica_voxel.hpp"
#include "color.hpp"
#include "point.hpp"
#include "profiling.hpp"

namespace putils::magica_voxel {
	struct vertex_data {
		float color[3] = { 0.f, 0.f, 0.f };

		bool operator==(size_t i) const {
			return color[0] == 0.f && color[1] == 0.f && color[2] == 0.f;
		}

		bool operator>(size_t i) const {
			return color[0] != 0.f || color[1] != 0.f || color[2] != 0.f;
		}

		bool operator==(const vertex_data & rhs) const {
			return color[0] == rhs.color[0] && color[1] == rhs.color[1] && color[2] == rhs.color[2];
		}
	};

	namespace detail {
		static auto build_mesh(PolyVox::RawVolume<vertex_data> & volume) noexcept {
			PUTILS_PROFILING_SCOPE;

			const auto encoded_mesh = PolyVox::extractCubicMesh(&volume, volume.getEnclosingRegion());
			const auto mesh = PolyVox::decodeMesh(encoded_mesh);
			return mesh;
		}

		using mesh_type = decltype(build_mesh(PolyVox::RawVolume<vertex_data>{ {} }));

		static bool id_matches(const char * s1, const char * s2) noexcept {
			return strncmp(s1, s2, 4) == 0;
		}

		template<typename T>
		static void read_from_stream(T & header, std::istream & s, unsigned int size) noexcept {
			PUTILS_PROFILING_SCOPE;

			s.read((char *)&header, size);
			assert(s.gcount() == size);
		}

		template<typename T>
		static void read_from_stream(T & header, std::istream & s) noexcept {
			read_from_stream(header, s, sizeof(header));
		}

		static void check_header(std::istream & s) noexcept {
			PUTILS_PROFILING_SCOPE;

			magica_voxel::file_header header;
			read_from_stream(header, s);
			assert(id_matches(header.id, "VOX "));
			assert(header.version_number == 150);
		}
	}

	static auto load_vox_file(const char * f, putils::point3i * out_size = nullptr) noexcept {
		PUTILS_PROFILING_SCOPE;

#ifndef KENGINE_NDEBUG
		std::cout << "[magica_voxel] Loading " << f << "...";
#endif
		std::ifstream stream(f, std::ios::binary);
		assert(stream);
		detail::check_header(stream);

		magica_voxel::chunk_header main;
		detail::read_from_stream(main, stream);
		assert(detail::id_matches(main.id, "MAIN"));

		magica_voxel::chunk_header first;
		detail::read_from_stream(first, stream);
		assert(detail::id_matches(first.id, "SIZE")); // "PACK" not yet supported
		assert(first.children_bytes == 0);
		assert(first.content_bytes == sizeof(magica_voxel::chunk_content::size));

		magica_voxel::chunk_content::size size;
		detail::read_from_stream(size, stream);
		if (out_size != nullptr) {
			out_size->x = size.x;
			out_size->y = size.y;
			out_size->z = size.z;
		}

		PolyVox::RawVolume<vertex_data> volume(
			PolyVox::Region{
				// clang-format off
				{ 0, 0, 0, },
				// clang-format on
				{ size.x, size.z, size.y } });

		magica_voxel::chunk_header voxels_header;
		detail::read_from_stream(voxels_header, stream);
		assert(detail::id_matches(voxels_header.id, "XYZI"));

		magica_voxel::chunk_content::rgba rgba;

		magica_voxel::chunk_content::xyzi xyzi;
		detail::read_from_stream(xyzi, stream);
		assert(voxels_header.content_bytes == sizeof(xyzi) + xyzi.num_voxels * sizeof(int));

		for (int i = 0; i < xyzi.num_voxels; ++i) {
			magica_voxel::chunk_content::xyzi::voxel voxel;
			detail::read_from_stream(voxel, stream);

			const auto & color = rgba.palette[voxel.color_index];
			const vertex_data voxel_value{ { (float)color.r / 255.f, (float)color.g / 255.f, (float)color.b / 255.f } };
			volume.setVoxel(voxel.x, voxel.z, voxel.y, voxel_value);
		}

#ifndef KENGINE_NDEBUG
		std::cout << " Done." << std::endl;
#endif

		return detail::build_mesh(volume);
	}
}