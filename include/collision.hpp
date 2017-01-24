// #ifndef COLLISION_HPP
// #define COLLISION_HPP
//
// #include <glm/glm.hpp>
// #include <glm/gtx/transform.hpp>
//
// struct Box;
//
// enum struct ColliderType
// {
//     BOX
// };
//
// struct Collider {
//     virtual ColliderType collider_type() const = 0;
//     virtual bool intersects
//     (
//         glm::vec3 const& thispos,
//         Collider const& other,
//         glm::vec3 const& otherpos
//     ) = 0;
//     virtual glm::vec3 correct
//     (
//         glm::vec3 const& thispos,
//         Collider const& other,
//         glm::vec3 const& otherpos,
//         glm::vec3 const& othervel
//     ) = 0;
// };
//
// // struct None : public Collider {
// //     None() {}
// //
// //     ColliderType collider_type() const override
// //         { return ColliderType::NONE; }
// //
// //     virtual bool intersects
// //     (
// //         glm::vec3 const& thispos,
// //         Collider const& other,
// //         glm::vec3 const& otherpos
// //     ) { return false; };
// //     virtual glm::vec3 correct
// //     (
// //         glm::vec3 const& thispos,
// //         Collider const& other,
// //         glm::vec3 const& otherpos
// //     ) { return otherpos; };
// // };
//
// struct Box : public Collider {
//     glm::vec3 offset;
//     Box(glm::vec3 offset)
//         : offset(offset) {}
//
//     ColliderType collider_type() const override
//         { return ColliderType::BOX; }
//
//     bool intersects
//     (
//         glm::vec3 const& thispos,
//         Collider const& other,
//         glm::vec3 const& otherpos
//     ) override;
//     glm::vec3 correct
//     (
//         glm::vec3 const& thispos,
//         Collider const& other,
//         glm::vec3 const& otherpos,
//         glm::vec3 const& othervel
//     );
//     bool intersects
//     (
//         glm::vec3 const& thispos,
//         Box const& other,
//         glm::vec3 const& otherpos
//     );
//     glm::vec3 correct
//     (
//         glm::vec3 const& thispos,
//         Box const& other,
//         glm::vec3 const& otherpos,
//         glm::vec3 const& othervel
//     );
//
//
//     glm::vec3 correct_old
//     (
//         glm::vec3 const& thispos,
//         Box const& other,
//         glm::vec3 const& otherpos
//     );
// };
//
// #endif /* end of include guard: COLLISION_HPP */
