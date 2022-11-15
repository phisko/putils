#pragma once

#define PUTILS_DEFAULT_COPY(T) \
	T(const T &) noexcept = default; \
	T & operator=(const T &) noexcept = default;

#define PUTILS_DELETE_COPY(T) \
	T(const T &) noexcept = delete; \
	T & operator=(const T &) noexcept = delete;

#define PUTILS_DEFAULT_MOVE(T) \
	T(T &&) noexcept = default; \
	T & operator=(T &&) noexcept = default;

#define PUTILS_DELETE_MOVE(T) \
	T(T &&) noexcept = delete; \
	T & operator=(T &&) noexcept = delete;

#define PUTILS_MOVE_ONLY(T) \
	T() noexcept = default; \
	PUTILS_DELETE_COPY(T) \
	PUTILS_DEFAULT_MOVE(T)
