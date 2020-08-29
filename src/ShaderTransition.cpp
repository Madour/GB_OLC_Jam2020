// Created by Modar Nasser on 29/08/2020.

#include "ShaderTransition.hpp"

#include <utility>


ShaderInTransition::ShaderInTransition(std::string uniform_name) :
m_uniform_name(std::move(uniform_name)),
m_shader(app->getShader()),
m_treshold(1.f)
{}

void ShaderInTransition::onUpdate() {
    m_treshold -= 0.025f;
    if (m_treshold <= 0.0) {
        end();
        m_treshold = 0.;
    }
    if (m_shader)
        m_shader->setUniform(m_uniform_name, m_treshold);
}


ShaderOutTransition::ShaderOutTransition(std::string uniform_name) :
m_uniform_name(std::move(uniform_name)),
m_shader(app->getShader()),
m_treshold(0.)
{}

void ShaderOutTransition::onUpdate() {
    m_treshold += 0.025f;
    if (m_treshold >= 1.5)
        end();
    if (m_shader)
        m_shader->setUniform(m_uniform_name, m_treshold);
}
