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
    m_palette_shader->setUniformArray("colors", Palette::getColor(Palette::Color::Black).data(), 4);
    setShader(m_palette_shader.get());

    // setting fonts
    initBitmapFonts();

    m_map.loadFromFile("assets/warehouse.tmx");
    for (const auto& rect : m_map.getObjectLayer("collisions")->getRectangles())
        MapCollisions::add(ns::FloatRect(rect.getShape().getGlobalBounds()));

    auto shape = std::make_shared<sf::RectangleShape>();
    shape->setSize({160, 144});

    auto rect = std::make_shared<sf::RectangleShape>();
    rect->setSize({16, 16});
    rect->setPosition(32, 32);
    rect->setTexture(&ns::Res::getTexture("pokemon.png"));
    rect->setTextureRect({0, 0, 16, 16});

    m_textbox = std::make_shared<TextBox>("Textbox is stylish and very nice ! that's cool ! Let's test the 3 lines limit; okay that looks good so far.", m_fonts["default"]);

    m_player = std::make_shared<Player>();
    m_player->setPosition(50, 50);

    //---------------------------------------------------//

    m_scene = createScene("main");
    m_scene->addLayer("ground", 0);
    m_scene->addLayer("front", 1);
    m_scene->addLayer("top", 2);

    m_ui_scene = createScene("ui");


    m_camera = createCamera("main", 0, {0, 0, ns::Config::Window::view_width, ns::Config::Window::view_height});
    m_camera->lookAt(m_scene);
    m_camera->follow(*m_player);
    m_camera->setLimitsRectangle(ns::IntRect(m_map.getTileLayer("ground")->getGlobalBounds()));

    m_ui_camera = createCamera("ui", 2, {0, 0, ns::Config::Window::view_width, ns::Config::Window::view_height});
    m_ui_camera->lookAt(m_ui_scene);

    //---------------------------------------------------//

    m_scene->getLayer("ground")->add(m_map.getTileLayer("ground"));
    m_scene->getLayer("front")->add(m_map.getTileLayer("front"));
    m_scene->getLayer("front")->add(m_player);
    m_scene->getLayer("top")->add(m_map.getTileLayer("top"));
    m_scene->getLayer("top")->add(m_map.getObjectLayer("collisions"));

    m_ui_scene->getDefaultLayer()->add(m_textbox);

    ns::DebugTextInterface::color = sf::Color::Black;
    ns::DebugTextInterface::outline_color = sf::Color::White;
    ns::DebugTextInterface::outline_thickness = 1;
    addDebugText<int>(&m_ticks, "ticks:", {0, 0});

}

void Game::onEvent(const sf::Event& event) {
    App::onEvent(event);
    if (m_textbox)
        m_textbox->onEvent(event);
    switch (event.type) {
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::F1) {
                ns::Config::debug = !ns::Config::debug;
                m_map.getObjectLayer("collisions")->setVisible(ns::Config::debug);
            }
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
                tr->setOnEndCallback([&](){
                    auto tr = new ShaderInTransition("threshold");
                    tr->start();
                    tr->setOnEndCallback([&](){
                        m_textbox = std::make_shared<TextBox>("Date : -2000 before JC. \nLocation : Egypt",m_fonts["italic"]);
                        m_ui_scene->getDefaultLayer()->add(m_textbox);
                    });
                });
            }

            break;

        default:
            break;
    }
}

void Game::update() {
    m_player->inputs()->setCaptureInput(true);
    if (m_textbox != nullptr) {
        m_textbox->update();
        if (m_textbox->closed()) {
            m_ui_scene->getDefaultLayer()->remove(m_textbox);
            m_textbox = nullptr;
        }
        m_player->inputs()->setCaptureInput(false);
    }

    m_player->update();
    m_ticks++;

}

void Game::setPalette(Palette::Color color) {
    m_palette_shader->setUniformArray("colors", Palette::getColor(color).data(), 4);
}

void Game::setPalette(int color) {
    m_palette_shader->setUniformArray("colors", Palette::getColor((Palette::Color)color).data(), 4);
}

void Game::initBitmapFonts() {

    auto char_map = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNO"
                    "PQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ €";

    auto* default_font = new ns::BitmapFont(
            ns::Res::get().in("fonts").getTexture("default.png"),
            {8, 8},
            char_map,
            {
                {"@", 8},
                {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 7},
                {"#$123456789", 7},
                {"j", 5},
                {"il", 4},
                {".,!:;", 2},
            },
            {6, 8}
    );
    auto* italic_font = new ns::BitmapFont(
            ns::Res::get().in("fonts").getTexture("italic.png"),
            {8, 8},
            char_map,
            {
                    {"@", 8},
                    {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 7},
                    {"#$123456789", 7},
                    {"j", 5},
                    {"il", 4},
                    {".,!:;", 3},
            },
            {6, 8}
    );
    auto* bold_font = new ns::BitmapFont(
            ns::Res::get().in("fonts").getTexture("bold.png"),
            {8, 8},
            char_map,
            {
                    {"@", 8},
                    {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 8},
                    {"#$123456789", 8},
                    {"j", 5},
                    {"il", 5},
                    {".,!:;", 3},
            },
            {6, 8}
    );
    m_fonts["default"] = std::shared_ptr<ns::BitmapFont>(default_font);
    m_fonts["italic"] = std::shared_ptr<ns::BitmapFont>(italic_font);
    m_fonts["bold"] = std::shared_ptr<ns::BitmapFont>(bold_font);
}