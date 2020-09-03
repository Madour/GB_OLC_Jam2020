// Created by Modar Nasser on 29/08/2020.

#include "Game.hpp"
#include "GameAccess.hpp"

#include "GameState.hpp"
#include "states/TitleScreenState.hpp"
#include "states/MuseumLevelState.hpp"


void resolveCollision(ns::BaseEntity* ent, const ns::FloatRect& rect) {
    ns::FloatRect intersection;
    auto col = ns::FloatRect(ent->collider()->getCollision().getShape().getGlobalBounds());
    if (col.intersects(rect, intersection)) {
        if (rect.width == 1 && ent->physics()->getDirection() == sf::Vector2i(0, -1)) {
            if (std::abs(col.left - rect.left) > std::abs(col.right() - rect.right())) {
                ent->setX(rect.left - col.width/2);
            }
            else if (std::abs(col.left - rect.left) < std::abs(col.right() - rect.right())) {
                ent->setX(rect.right() + col.width/2);
            }
        }
        else {
            if (intersection.width < intersection.height && intersection.height >= 3) {
                // entity on left side of box
                if (std::abs(col.left - rect.right()) >= std::abs(col.right() - rect.left))
                    ent->setX(ent->getX() - intersection.width);
                // entity right side of box
                else
                    ent->setX(ent->getX() + intersection.width);
            }
            else if (intersection.width >= 3) {
                // entity over box
                if (std::abs(col.top - rect.bottom()) >= std::abs(col.bottom() - rect.top))
                    ent->setY(ent->getY() - intersection.height);
                // entity under box
                else
                    ent->setY(ent->getY() + intersection.height);
            }
        }
        ent->collider()->update();
    }
}

Game::Game() : ns::App("GB_OLC_Jam2020", {160, 144}, 5) {
    std::srand((int) std::time(nullptr));
    GameState::game = this;
    GameAccess::game = this;

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
                "new_col = vec4(colors[0]/255., col.a);"
            "else if (col.r <= threshold + 0.4f)"
                "new_col = vec4(colors[1]/255., col.a);"
            "else if (col.r <= threshold + 0.72f)"
                "new_col = vec4(colors[2]/255., col.a);"
            "else if (col.r <= threshold + 1.f)"
                "new_col = vec4(colors[3]/255., col.a);"

            "gl_FragColor = new_col;"
        "}",
        sf::Shader::Fragment
    );
    m_palette_shader->setUniform("threshold", 0.f);
    m_palette_shader->setUniformArray("colors", Palette::getColor(Palette::Color::Black).data(), 4);
    setShader(m_palette_shader.get());

    m_wave_shader = std::make_shared<sf::Shader>();
    m_wave_shader->loadFromMemory(
            "uniform sampler2D texture;"
            "uniform float time;"
            "void main()"
            "{"
                "vec2 pos = gl_TexCoord[0].xy;"
                "pos.x = pos.x + sin(pos.y*time*10. + time*5.)/20.;"
                "vec4 col = texture2D(texture, pos);"
                "vec4 new_col = col;"
                "gl_FragColor = new_col;"
            "}",
            sf::Shader::Fragment
    );
    m_wave_shader->setUniform("time", 0.f);

    // setting fonts
    initBitmapFonts();

    player = std::make_shared<Player>();
    player->setPosition(184, 45);

    hud = std::make_shared<HUD>();

    //---------------------------------------------------//

    scene = createScene("main");
    scene->addLayer("back", 0);
    scene->addLayer("front", 1);
    scene->addLayer("entities", 2);
    scene->addLayer("top", 3);
    scene->addLayer("shapes", 4);

    ui_scene = createScene("ui");
    ui_scene->addLayer("hud", 1);
    ui_scene->getLayer("hud")->add(hud);

    camera = createCamera("main", 0, {0, 0, ns::Config::Window::view_width, ns::Config::Window::view_height});
    camera->lookAt(scene);
    camera->setShader(m_wave_shader.get());
    camera->toggleShader();

    ui_camera = createCamera("ui", 2, {0, 0, ns::Config::Window::view_width, ns::Config::Window::view_height});
    ui_camera->lookAt(ui_scene);

    //---------------------------------------------------//

    m_state = new TitleScreenState();
    m_state->init();

    ns::DebugTextInterface::color = sf::Color::Black;
    ns::DebugTextInterface::outline_color = sf::Color::White;
    ns::DebugTextInterface::outline_thickness = 1;
    addDebugText<int>(&m_ticks, "ticks:", {0, 0});

    addDebugText<sf::Vector2f>([&](){return player->getPosition();}, "pos:", {0, 15});
    addDebugText<unsigned int>([&](){return ns::Transition::list.size();}, "transitions:", {0, 30});

}

void Game::onEvent(const sf::Event& event) {
    App::onEvent(event);
    m_state->onEvent(event);

    switch (event.type) {
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::F1) {
                ns::Config::debug = !ns::Config::debug;
            }
            if (event.key.code == sf::Keyboard::F2)
                toggleShader();
            if (event.key.code == sf::Keyboard::F3) {
                m_palette_index = (m_palette_index + 1 ) % (int)Palette::Color::count;
                setPalette(m_palette_index);
            }
            if (event.key.code == sf::Keyboard::F4) {
                hud->open(); setState<LevelState>("egypt_out.tmx");
            }
            if (event.key.code == sf::Keyboard::F5) {
                hud->close();
            }
            if (event.key.code == sf::Keyboard::F6) {
                player->damage();
            }

            if (event.key.code == sf::Keyboard::Escape)
                getWindow().close();
            if (event.key.code == sf::Keyboard::F)
                toggleFullscreen();

            if (event.key.code == ns::Config::Inputs::getButtonKey("select")) {
                if (hud->isOpened())
                    hud->nextItem();
            }

            break;

        default:
            break;
    }
}

void Game::update() {
    m_state->update();
    hud->update();

    for (auto& ent : Enemy::list) {
        ent->update();
    }
    scene->getLayer("entities")->ySort();
    m_ticks++;
}

void Game::setPalette(Palette::Color color) {
    m_palette_shader->setUniformArray("colors", Palette::getColor(color).data(), 4);
}

void Game::setPalette(int color) {
    m_palette_shader->setUniformArray("colors", Palette::getColor((Palette::Color)color).data(), 4);
}

bool Game::isTextboxOpened() {
    return m_state->m_textbox != nullptr;
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
                {"#$23456789", 8},
                {"j", 5},
                {"il", 5},
                {".,", 5},
                {":!;1", 6}
            },
            {6, 8}
    );
    fonts["default"] = std::shared_ptr<ns::BitmapFont>(default_font);
    fonts["italic"] = std::shared_ptr<ns::BitmapFont>(italic_font);
    fonts["bold"] = std::shared_ptr<ns::BitmapFont>(bold_font);
}

Game::~Game() {
    delete(m_state);
}

