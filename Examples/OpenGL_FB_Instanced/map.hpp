#pragma once


#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/texture_manager.hpp>
#include <OpenGL/image_data.hpp>
#include <OpenGL/image_manager.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/camera.hpp>

using maybe_texdata_t = opengl::TextureManager::maybe_texture_t<
    opengl::texture_data_t
>;
using vertex_t = opengl::vec3pos_vec2tex_t;


std::vector<vertex_t> create_vertices_xz() {
    return {
        {{-1.0, 0.0, -1.0}, {0, 1}},
        {{ 1.0, 0.0, -1.0}, {1, 1}},
        {{ 1.0, 0.0,  1.0}, {1, 0}},
        {{-1.0, 0.0,  1.0}, {0, 0}}
    };
}

std::vector<GLuint> create_indices() {
    return {
        3, 0, 1,
        1, 2, 3
    };
}


struct Biome final {
    enum ID : uint32_t {
        NONE = 0,
        ICE           = 1 << 0,
        TUNDRA        = 1 << 1,
        FOREST        = 1 << 2,
        GRASSLANDS    = 1 << 3,
        MEDITERRANEAN = 1 << 4,
        DESERT        = 1 << 5,
        OCEAN         = 1 << 6
    };

    static const char* to_string(Biome::ID id) {
        switch (id) {
        case ICE:           return "ice";
        case TUNDRA:        return "tundra";
        case FOREST:        return "forest";
        case GRASSLANDS:    return "grasslands";
        case MEDITERRANEAN: return "mediterranean";
        case DESERT:        return "desert";
        case OCEAN:         return "ocean";
        default:            return "none";
        }
    }
};
using B = Biome::ID;

std::unordered_map<uint32_t, glm::u8vec4> biome_to_map {
    {Biome::ICE,           {255, 255, 255, 255}},
    {Biome::TUNDRA,        {0,   70,  0,   255}},
    {Biome::FOREST,        {34,  139, 34,  255}},
    {Biome::GRASSLANDS,    {218, 165, 32,  255}},
    {Biome::MEDITERRANEAN, {60,  179, 113, 255}},
    {Biome::DESERT,        {210, 180, 140, 255}},
    {Biome::OCEAN,         {0,   105, 148, 255}},
    {Biome::NONE,          {0,   0,   0,   255}}
};

std::unordered_map<uint32_t, float> biome_frames {
    {Biome::ICE,           0.0f},
    {Biome::TUNDRA,        1.0f},
    {Biome::FOREST,        2.0f},
    {Biome::GRASSLANDS,    3.0f},
    {Biome::MEDITERRANEAN, 4.0f},
    {Biome::DESERT,        5.0f},
    {Biome::OCEAN,         6.0f}
};

struct NaturalResources final {
    enum ID : uint32_t {
        NONE = 0,

        IRON_ORE = 1 << 0,
        STONE    = 1 << 1,
        CLAY     = 1 << 2
    };

    static const char* to_string(NaturalResources::ID id) {
        switch (id) {
        case IRON_ORE: return "iron";
        case STONE:    return "stone";
        case CLAY:     return "clay";
        }
    }
};
using NR = NaturalResources::ID;

std::unordered_map<uint32_t, glm::u8vec4> resources_to_map {
    {NR::IRON_ORE, {160, 160, 160, 255}},
    {NR::STONE,    {102, 118, 134, 255}},
    {NR::CLAY,     {210, 180, 140, 255}},
    {NR::NONE,     {0,   0,   0,   255}}
};

std::unordered_map<uint32_t, float> resources_frames {
    {NR::IRON_ORE, 0.0f},
    {NR::STONE,    1.0f},
    {NR::CLAY,     2.0f},
    {NR::NONE,     3.0f}
};


struct Cell final {
    Biome::ID biome                {Biome::NONE};
    NaturalResources::ID resources {NaturalResources::NONE};
};


static std::vector<Cell> map_4x4 {
    {B::ICE, NR::NONE}, {B::ICE, NR::NONE}, {B::ICE, NR::NONE},
    {B::ICE, NR::NONE},

    {B::OCEAN, NR::NONE}, {B::OCEAN, NR::NONE}, {B::TUNDRA, NR::IRON_ORE},
    {B::TUNDRA, NR::NONE},

    {B::OCEAN, NR::NONE}, {B::FOREST, NR::CLAY}, {B::FOREST, NR::STONE},
    {B::OCEAN, NR::NONE},

    {B::GRASSLANDS, NR::NONE}, {B::MEDITERRANEAN, NR::NONE},
    {B::DESERT, NR::IRON_ORE}, {B::DESERT, NR::NONE}
};

std::vector<float> biomes_map_frames(const std::vector<Cell>& map) {
    std::vector<float> out(map.size());
    for (size_t i = 0; i < map.size(); ++i) {
        const auto& cell = map[i];
        out[i] = biome_frames[cell.biome];
    }
    return out;
}

std::vector<float> resources_map_frames(const std::vector<Cell>& map) {
    std::vector<float> out(map.size());
    for (size_t i = 0; i < map.size(); ++i) {
        const auto& cell = map[i];
        out[i] = resources_frames[cell.resources];
        std::cout << out[i] << std::endl;
    }
    return out;
}


struct map_chunk_t final {
    using maybe_tex_manager_t = std::optional<std::reference_wrapper<
        opengl::TextureManager>
    >;

    glm::mat4 rotation, scale, translation;
    std::vector<glm::mat4> cell_models;

    opengl::instant_render_data_t main_render;
    int cells_w_count, cells_h_count;
    maybe_tex_manager_t tex_manager = std::nullopt;
    std::string tex_id = "biomes";

public:
    static map_chunk_t create(int cwc, int chc,
                              const glm::vec3& pos,
                              maybe_tex_manager_t tex_manager) {
        map_chunk_t self;
        self.tex_manager = tex_manager;
        self.translation = glm::translate(glm::mat4(1.0), pos);
        self.scale = glm::mat4(1.0);
        self.rotation = glm::mat4(1.0);

        self.main_render = opengl::instant_render_data_t::create(
            std::filesystem::path("./vec3_vec2_mat4_f.vert"),
            std::filesystem::path("./samp_2D_array.frag"),
            create_vertices_xz(),
            create_indices()
        );
        self.cells_w_count = cwc;
        self.cells_h_count = chc;
        self.gen_cell_models();
        self.main_render.buffers["models"] =
            opengl::mat4_instanced::gen_buffer(
                self.main_render.impl.vao,
                opengl::mat4_instanced::convert(self.cell_models),
                2
            );
        self.main_render.buffers["frames"] =
            opengl::float_instanced::gen_buffer(
                self.main_render.impl.vao,
                opengl::float_instanced::convert(biomes_map_frames(map_4x4)),
                6
            );
        self.main_render.instance_count = cwc * chc;

        return self;
    }

    ~map_chunk_t() {
        main_render.free();
    }

    glm::mat4 model() const {
        return translation * rotation * scale;
    }

    opengl::texture_activation_command_t tex_activation() const {
        if (!tex_manager.has_value()) {
            throw std::runtime_error("No texture manager");
        }
        auto maybe_tex = tex_manager->get().get<opengl::texture_data_array_2d_t>(
            tex_id
        );
        if (!maybe_tex.has_value()) {
            throw std::runtime_error("No texture found");
        }
        const auto& tex = maybe_tex.value().get();
        return {
            .tex_unit     = GL_TEXTURE0,
            .sampler_type = tex.tex_data.target,
            .id           = tex.tex_data.id,
            .program      = main_render.impl.program,
            .sampler_name = "tex_array"
        };
    }

    void render(const opengl::Camera& cam) {
        auto pr = main_render.impl.program;

        opengl::use(pr);
        opengl::buffer_bind_guard bind_vao({ .vao = main_render.impl.vao });
        opengl::activate_texture(tex_activation());
        opengl::set_mat4(pr, "projection", cam.projection());
        opengl::set_mat4(pr, "view", cam.view());
        opengl::draw_instance_elements({
            .vao           = main_render.impl.vao,
            .count         = main_render.impl.ebo_count,
            .instancecount = main_render.instance_count
        });
        opengl::use(0);
    }

private:
    void gen_cell_models() {
        const size_t area = cells_w_count * cells_h_count;
        const glm::mat4 offset = glm::translate(
            glm::mat4(1.0),
            glm::vec3(-(cells_w_count / 2.0f), 0.0, -(cells_h_count / 2.0f))
        );

        cell_models.clear();
        cell_models = std::vector<glm::mat4>(area);
        glm::mat4 scale = glm::scale(glm::mat4(1.0), { 0.49, 0.0, 0.49 });
        for (uint32_t i = 0; i < area; ++i) {
            const uint32_t x = i % cells_w_count;
            const uint32_t z = i / cells_w_count;
            glm::mat4 loc_model = glm::translate(
                glm::mat4(1.0),
                glm::vec3(x + 0.5f, 0.0, z + 0.5f)
            );
            cell_models[i] = model() * ((offset * loc_model) * scale);
        }
    }
};


struct IOHandler final : public ui::Listener {
public:
    IOHandler(map_chunk_t& map_chunk)
        : map_chunk_(map_chunk)
    {
        ui::io::IO::instance().subscribe(this);
    }

    void consume(const ui::KeyEvent& event) override {
        if (event.key == GLFW_KEY_SPACE && event.action == GLFW_RELEASE) {
            opengl::float_instanced::update(
                map_chunk_.main_render.buffers["frames"],
                opengl::float_instanced::convert(resources_map_frames(map_4x4))
            );
            map_chunk_.tex_id = map_chunk_.tex_id == "biomes" ?
                "resources" : "biomes";
        }
    }

    void consume(const ui::MouseEvent& event) override {}
    void consume(const ui::MouseButtonEvent& event) override {}
    void consume(const ui::ScrollEvent& event) override {}
    void consume(const ui::DropEvent& event) override {}
    void consume(const ui::FramebufferEvent& event) override {}

private:
    map_chunk_t& map_chunk_;
};
