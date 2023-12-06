#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <glm/glm.hpp>
#include <iostream>


    class camera {
        private:
            float m_near, m_far, m_aspect_ratio;
        public:

            glm::vec3 m_up, m_dir, m_side, m_pos;
            camera(float t_near, float t_far, glm::vec3 t_dir, glm::vec3 t_pos) : m_near(t_near), m_far(t_far), m_dir(t_dir), m_pos(t_pos) {
                rebase();
            }
            void rebase() {
                m_side = glm::normalize(glm::cross(m_dir, {0, 1, 0}));
                m_up = glm::normalize(glm::cross(m_side, m_dir));
            }
            void set_aspect_ratio(float r) { m_aspect_ratio = r; }
            ~camera() {}
            glm::mat4 get_camera_matrix() const;
            glm::mat4 get_perspective_matrix() const;
    };


#endif