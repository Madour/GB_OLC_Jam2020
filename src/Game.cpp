// Created by Modar Nasser on 29/08/2020.

#include "Game.hpp"

Game::Game() : ns::App("GB_OLC_Jam2020", {160, 144}, 5) {

    // setting up shader
    m_palette_shader = std::make_shared<sf::Shader>();
    m_palette_shader->loadFromMemory(
        "uniform sampler2D texture;"
        "uniform float threshold;"
        "uniform vec3 colors[4];"
        "void main()"
        "{"
            "vec2 pos = gl_TexCoord[0].xy;"
            "vec4 col = texture2D(texture, pos);"
            "vec4 new_col = col;"
            "if (col.r <= threshold)"
                "new_col = vec4(colors[0], col.a);"
            "else if (col.r <= threshold + 0.4f)"
                "new_col = vec4(colors[1], col.a);"
            "else if (col.r <= threshold + 0.72f)"
                "new_col = vec4(colors[2], col.a);"
            "else if (col.r <= threshold + 1.f)"
                "new_col = vec4(colors[3], col.a);"
            "gl_FragColor = new_col;"
        "}",
        sf::Shader::Fragment
    );
    m_palette_shader->setUniform("threshold", 0.f);
    m_palette_shader->setUniformArray("colors", Palette::getColor(Palette::Color::Green).data(), 4);
    setShader(m_palette_shader.get());

    // setting fonts


    auto shape = std::make_shared<sf::RectangleShape>();
    shape->setSize({160, 144});

    auto rect = std::make_shared<sf::RectangleShape>();
    rect->setSize({16, 16});
    rect->setPosition(32, 32);
    rect->setTexture(&ns::Res::getTexture("pokemon.png"));
    rect->setTextureRect({0, 0, 16, 16});

    m_player = std::make_shared<Player>();
    m_player->setPosition(50, 50);

    //---------------------------------------------------//

    m_scene = createScene("main");
    m_scene->addLayer("shapes", 1);
    m_scene->getLayer("shapes")->add(shape);
    m_scene->getLayer("shapes")->add(m_player);

    //---------------------------------------------------//

    m_camera = createCamera("main", 0, {0, 0, ns::Config::Window::view_width, ns::Config::Window::view_height});
    m_camera->lookAt(m_scene);

    //---------------------------------------------------//

    ns::DebugTextInterface::color = sf::Color::Black;
    ns::DebugTextInterface::outline_color = sf::Color::White;
    ns::DebugTextInterface::outline_thickness = 1;
    addDebugText<int>(&m_ticks, "ticks:", {0, 0});
}

void Game::onEvent(const sf::Event& event) {
    App::onEvent(event);
    switch (event.type) {
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::F1)
                ns::Config::debug = !ns::Config::debug;
            if (event.key.code == sf::Keyboard::F2)
                toggleShader();
            if (event.key.code == sf::Keyboard::F3) {
                m_palette_index = (m_palette_index + 1 ) % (int)Palette::Color::count;
                setPalette(m_palette_index);
            }

            if (event.key.code == sf::Keyboard::Escape)
                getWindow().close();
            if (event.key.code == sf::Keyboard::F)
                toggleFullscreen();
            if (event.key.code == sf::Keyboard::T) {
                auto tr = new ShaderOutTransition("threshold");
                tr->start();
                tr->setOnEndCallback([](){
                    auto tr = new ShaderInTransition("threshold");
                    tr->start();
                });
            }

            break;

        default:
            break;
    }
}

void Game::update() {
    m_player->update();
    m_ticks++;

}

void Game::setPalette(Palette::Color color) {
    m_palette_shader->setUniformArray("colors", Palette::getColor(color).data(), 4);
}

void Game::setPalette(int color) {
    m_palette_shader->setUniformArray("colors", Palette::getColor((Palette::Color)color).data(), 4);
}

void Game::createFont(const std::string& name) {
    auto* font = new ns::BitmapFont(
            ns::Res::get().in("fonts").getTexture(name+".png"),
            sf::Vector2u(8, 8),
            " !\"#$%&'()*+, ./"
            "0123456789:;<=>?"
            "@ABCDEFGHIJKLMNO"
            "PQRSTUVWXYZ[\\]^_"
            "`abcdefghijklmno"
            "pqrstuvwxyz{|}~ "
            "€ ,  þ     <    "
            "   “” -─   >    "
            "   £¤  §   «¬   "
            "° ²  µ · ¹ »   ",
            std::unordered_map<char, int>({{'i', 4}, {'l', 4}, {'j', 5}})
    );
    m_fonts[name] = std::shared_ptr<ns::BitmapFont>(font);
}