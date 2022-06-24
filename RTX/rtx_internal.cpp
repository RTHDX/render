#include <ostream>

#include <glm/gtx/string_cast.hpp>

#include "utils.hpp"
#include "rtx_internal.hpp"

namespace rtx {

Ray::Ray(const Point& origin, const Vector& direction)
    : origin(origin)
    , direction(direction)
{}

Point Ray::at(float t) const { return origin + t * direction; }

std::ostream& operator << (std::ostream& os, const Ray& ray) {
    return os << "<Ray .origin: " << glm::to_string(ray.origin)
              << " .direction: " << glm::to_string(ray.direction);
}


Light::Light(const Point& position, float intensity)
    : position(position)
    , intensity(intensity)
{}

float Light::diffuse(const Point& hit_point, const Vector& normal) const {
    Vector direction = glm::normalize(position - hit_point);
    return intensity *
           std::max(0.0f, glm::dot(direction, normal));
}

float Light::specular(const Point& point, const Vector& normal,
                      const Vector& ray_direction, float exp) const {
    Vector reflection = reflect(glm::normalize(position - point), normal);
    return intensity *
           powf(std::max(0.0f, glm::dot(reflection, ray_direction)), exp);
}


Material::Material(const Albedo& albedo, const Color& color, float exponent,
                   float ref)
    : albedo(albedo)
    , diffuse_color(color)
    , specular_exponent(exponent)
    , refractive_index(ref)
{}


Vector reflect(const Vector& input, const Vector& normal) {
    return input - normal * 2.0f * glm::dot(input, normal);
}

Vector refract(const Vector& input, const Vector& normal, float refract) {
    float cosi = std::max(-1.0f, std::min(1.0f, glm::dot(input, normal)));
    float etai = 1.0f, etat = refract;
    Vector n = normal;
    if (cosi < 0.0) {
        cosi = -cosi;
        std::swap(etai, etat);
        n = -normal;
    }
    float eta = etai / etat;
    float k = 1.0 - eta * eta * (1 - cosi * cosi);
    return k < 0.0 ? Vector(0.0, 0.0, 0.0) :
        input * eta + n * (eta * cosi - sqrtf(k));
}


Hit::Hit(float t, Material const* material, const Point& point, const Vector& n)
    : t(t)
    , material(material)
    , point(point)
    , normal(n)
{}

bool Hit::is_valid() const {
    return !std::fabs(t - std::numeric_limits<float>::min() <
            std::numeric_limits<float>::epsilon());
}


Sphere::Sphere(const Point& center, const Material& material, float radius)
    : center(center)
    , material(material)
    , radius(radius)
{}

Hit Sphere::ray_intersect(const Ray& ray) const {
    Vector L = center - ray.origin;
    float tca = glm::dot(L, ray.direction);
    float d2 = glm::dot(L, L) - tca * tca;
    if (d2 > radius * radius) return Hit();
    float thc = sqrtf(radius * radius - d2);
    Hit hit;
    hit.t = tca - thc;
    float t1 = tca + thc;
    if (hit.t < 0) hit.t = t1;
    if (hit.t < 0) return Hit();
    hit.material = &material;
    hit.point = ray.at(hit.t);
    hit.normal = glm::normalize(hit.point - center);
    return hit;
}


Triangle::Triangle(const Point& a, const Point& b, const Point& c,
                   const Vector& n, const Material& material)
    : Object()
    , a(a)
    , b(b)
    , c(c)
    , normal(n)
    , material(material)
{}

Hit Triangle::ray_intersect(const Ray& ray) const {
    //return geometric(ray);
    return moeller_truembore(ray);
}

Hit Triangle::geometric(const Ray& ray) const {
    float n_dot_ray_dir = glm::dot(normal, ray.direction);
    if (utils::is_close(n_dot_ray_dir, 0.0f)) { return Hit(); }

    float d = -glm::dot(normal, a);
    float t = -(glm::dot(normal, ray.origin) + d) / n_dot_ray_dir;
    if (t < 0) { return Hit(); }
    Point p = ray.at(t);

    if (test_geometic(a, b, p)) { return Hit(); }
    if (test_geometic(b, c, p)) { return Hit(); }
    if (test_geometic(c, a, p)) { return Hit(); }

    return Hit(t, &material, p, normal);
}

bool Triangle::test_geometic(const Point& start, const Point& end,
                             const Point& hit) const {
    Vector edge = end - start;
    Vector temp = hit - end;
    Vector cross = glm::cross(temp, edge);
    float dot = glm::dot(normal, cross);
    return dot < 0.0f;
}

Hit Triangle::moeller_truembore(const Ray& ray) const {
    float ray_plate_dir = glm::dot(ray.direction, normal);
    if (ray_plate_dir > 1e-8) { return Hit(); }

    Vector v0v1 = make_edge(a, b);
    Vector v0v2 = make_edge(a, c);
    Vector pvec = glm::cross(ray.direction, v0v2);
    float det = glm::dot(v0v1, pvec);
    if (det < 1e-8) { return Hit(); }

    float inv_det = 1 / det;
    Vector tvec = ray.origin - a;

    float u = glm::dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1) { return Hit(); }

    Vector qvec = glm::cross(tvec, v0v1);
    float v = glm::dot(ray.direction, qvec) * inv_det;
    if (v < 0 || u + v > 1) { return Hit(); }

    float t = glm::dot(v0v2, qvec) * inv_det;
    auto p = ray.at(t);
    return Hit(t, &material, p, normal);
}


Mesh::Mesh(const std::vector<Triangle>& triangle)
    : Object()
    , faces(triangle)
{}

Hit Mesh::ray_intersect(const Ray& ray) const {
    static constexpr float LIMIT = 1000.0;
    float dist = std::numeric_limits<float>::max();

    Hit out_hit;
    for (const Triangle& face: faces) {
        Hit hit = face.ray_intersect(ray);
        bool is_valid = hit.is_valid();
        if (is_valid && hit.t < dist) {
            dist = hit.t;
            out_hit = hit;
        }
    }

    return dist < LIMIT ? out_hit : Hit();

    return Hit();
}

}
