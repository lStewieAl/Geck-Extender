#pragma once

template <typename T>
concept ni_is_zero_assignable_v = std::is_pointer_v<T> || std::is_enum_v<T> || requires(T a) { { a = 0 } -> std::same_as<T&>; };

#pragma region NiTMapBase

template <typename T_Key, typename T_Data>
struct NiTMapBaseRequirements {
    static_assert(std::default_initializable<T_Key>,
        "T_Key must be default constructible: T_Key::T_Key()");

    static_assert(std::copy_constructible<T_Key>,
        "T_Key must be copy constructible: T_Key::T_Key(const T_Key&)");

    static_assert(std::is_copy_assignable_v<T_Key>,
        "T_Key must support copy assignment: T_Key& operator=(const T_Key&)");

    static_assert(std::equality_comparable<T_Key>,
        "T_Key must support equality: bool T_Key::operator==(const T_Key&)");

    static_assert(std::is_pointer_v<T_Key> || std::is_convertible_v<T_Key, uintptr_t>,
        "T_Key must be a pointer, or convertible to uintptr_t");

    static_assert(std::default_initializable<T_Data>,
        "T_Data must be default constructible: T_Data::T_Data()");

    static_assert(std::copy_constructible<T_Data>,
        "T_Data must be copy constructible: T_Data::T_Data(const T_Data&)");

    static_assert(std::is_copy_assignable_v<T_Data>,
        "T_Data must support copy assignment: T_Data& operator=(const T_Data&)");
};

#pragma endregion

#pragma region NiTPointerListBase

template <typename T>
struct NiTPointerListBaseRequirements {
    static_assert(std::default_initializable<T>,
        "T must be default constructible, and the default constructor must produce the 'zero' (null sentinel) element");

    static_assert(ni_is_zero_assignable_v<T>,
        "T must support assignment from 0 or nullptr");

    static_assert(std::copy_constructible<T>,
        "T must be copy constructible");

    static_assert(std::destructible<T>,
        "T must be destructible");

    static_assert(std::equality_comparable<T>,
        "T must support equality and inequality comparison");

    static_assert(std::is_copy_assignable_v<T>,
        "T must support copy assignment");
};

#pragma endregion

#pragma region NiTArray

template <typename T>
struct NiTArrayRequirements {
    static_assert(std::default_initializable<T>,
        "T must be default constructible, and the default constructor must produce the 'zero' (null sentinel) element");

    static_assert(ni_is_zero_assignable_v<T>,
        "T must support assignment from 0 or nullptr");

    static_assert(std::copy_constructible<T>,
        "T must be copy constructible");

    static_assert(std::destructible<T>,
        "T must be destructible");

    static_assert(std::equality_comparable<T>,
        "T must support equality and inequality comparison");

    static_assert(std::is_copy_assignable_v<T>,
        "T must support copy assignment");
};

#pragma endregion

#pragma region NiTAllocator

template<typename T>
struct NiTAllocatorRequirements {
    static_assert(requires(T a) { { a.Allocate() } -> std::same_as<void*>; },
        "Allocator must provide a public member method: void* T::Allocate()");

    static_assert(requires(T a) { { a.Deallocate(static_cast<void*>(nullptr)) } -> std::same_as<void>; },
        "Allocator must provide a public member method: void T::Deallocate(void*)");
};

template<typename T, typename T_Data>
struct NiTArrayAllocatorRequirements {
    static_assert(requires{ { T::Allocate(0u) } -> std::same_as<T_Data*>; },
        "Allocator must provide a public static method: void* T::Allocate()");

    static_assert(requires{ { T::Deallocate(static_cast<T_Data*>(nullptr)) } -> std::same_as<void>; },
        "Allocator must provide a public static method: void T::Deallocate(void*)");
};

#pragma endregion