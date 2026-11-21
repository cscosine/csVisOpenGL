#include <QColor>
#include <csVisOpenGL/Colors.hpp>

namespace csVisOpenGL {
const Eigen::Vector4f Color::red = Eigen::Vector4f(0.96f, 0.26f, 0.21f, 1.0f);
const Eigen::Vector4f Color::pink = Eigen::Vector4f(0.91f, 0.12f, 0.39f, 1.0f);
const Eigen::Vector4f Color::purple = Eigen::Vector4f(0.61f, 0.15f, 0.69f, 1.0f);
const Eigen::Vector4f Color::deepPurple = Eigen::Vector4f(0.4f, 0.23f, 0.72f, 1.0f);
const Eigen::Vector4f Color::indigo = Eigen::Vector4f(0.25f, 0.32f, 0.71f, 1.0f);
const Eigen::Vector4f Color::blue = Eigen::Vector4f(0.13f, 0.59f, 0.95f, 1.0f);
const Eigen::Vector4f Color::lightBlue = Eigen::Vector4f(0.01f, 0.66f, 0.96f, 1.0f);
const Eigen::Vector4f Color::cyan = Eigen::Vector4f(0.0f, 0.74f, 0.83f, 1.0f);
const Eigen::Vector4f Color::teal = Eigen::Vector4f(0.0f, 0.59f, 0.53f, 1.0f);
const Eigen::Vector4f Color::green = Eigen::Vector4f(0.3f, 0.69f, 0.31f, 1.0f);
const Eigen::Vector4f Color::lightGreen = Eigen::Vector4f(0.55f, 0.76f, 0.29f, 1.0f);
const Eigen::Vector4f Color::lime = Eigen::Vector4f(0.8f, 0.86f, 0.22f, 1.0f);
const Eigen::Vector4f Color::yellow = Eigen::Vector4f(1.0f, 0.92f, 0.23f, 1.0f);
const Eigen::Vector4f Color::amber = Eigen::Vector4f(1.0f, 0.76f, 0.03f, 1.0f);
const Eigen::Vector4f Color::orange = Eigen::Vector4f(1.0f, 0.6f, 0.0f, 1.0f);
const Eigen::Vector4f Color::deepOrange = Eigen::Vector4f(1.0f, 0.34f, 0.13f, 1.0f);

const Eigen::Vector4f Color::white = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
const Eigen::Vector4f Color::black = Eigen::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);

const Eigen::Vector4f Color::palette[16] = {
    Eigen::Vector4f(0.96f, 0.26f, 0.21f, 1.0f), Eigen::Vector4f(0.91f, 0.12f, 0.39f, 1.0f),
    Eigen::Vector4f(0.61f, 0.15f, 0.69f, 1.0f), Eigen::Vector4f(0.4f, 0.23f, 0.72f, 1.0f),
    Eigen::Vector4f(0.25f, 0.32f, 0.71f, 1.0f), Eigen::Vector4f(0.13f, 0.59f, 0.95f, 1.0f),
    Eigen::Vector4f(0.01f, 0.66f, 0.96f, 1.0f), Eigen::Vector4f(0.0f, 0.74f, 0.83f, 1.0f),
    Eigen::Vector4f(0.0f, 0.59f, 0.53f, 1.0f),  Eigen::Vector4f(0.3f, 0.69f, 0.31f, 1.0f),
    Eigen::Vector4f(0.55f, 0.76f, 0.29f, 1.0f), Eigen::Vector4f(0.8f, 0.86f, 0.22f, 1.0f),
    Eigen::Vector4f(1.0f, 0.92f, 0.23f, 1.0f),  Eigen::Vector4f(1.0f, 0.76f, 0.03f, 1.0f),
    Eigen::Vector4f(1.0f, 0.6f, 0.0f, 1.0f),    Eigen::Vector4f(1.0f, 0.34f, 0.13f, 1.0f)};

const Eigen::Vector4f ColorDark::red = Eigen::Vector4f(0.72f, 0.11f, 0.11f, 1.0f);
const Eigen::Vector4f ColorDark::pink = Eigen::Vector4f(0.53f, 0.05f, 0.31f, 1.0f);
const Eigen::Vector4f ColorDark::purple = Eigen::Vector4f(0.29f, 0.08f, 0.55f, 1.0f);
const Eigen::Vector4f ColorDark::deepPurple = Eigen::Vector4f(0.19f, 0.11f, 0.57f, 1.0f);
const Eigen::Vector4f ColorDark::indigo = Eigen::Vector4f(0.1f, 0.14f, 0.49f, 1.0f);
const Eigen::Vector4f ColorDark::blue = Eigen::Vector4f(0.05f, 0.28f, 0.63f, 1.0f);
const Eigen::Vector4f ColorDark::lightBlue = Eigen::Vector4f(0.0f, 0.34f, 0.61f, 1.0f);
const Eigen::Vector4f ColorDark::cyan = Eigen::Vector4f(0.0f, 0.38f, 0.39f, 1.0f);
const Eigen::Vector4f ColorDark::teal = Eigen::Vector4f(0.0f, 0.3f, 0.25f, 1.0f);
const Eigen::Vector4f ColorDark::green = Eigen::Vector4f(0.11f, 0.37f, 0.13f, 1.0f);
const Eigen::Vector4f ColorDark::lightGreen = Eigen::Vector4f(0.2f, 0.41f, 0.12f, 1.0f);
const Eigen::Vector4f ColorDark::lime = Eigen::Vector4f(0.51f, 0.47f, 0.09f, 1.0f);
const Eigen::Vector4f ColorDark::yellow = Eigen::Vector4f(0.96f, 0.5f, 0.09f, 1.0f);
const Eigen::Vector4f ColorDark::amber = Eigen::Vector4f(1.0f, 0.44f, 0.0f, 1.0f);
const Eigen::Vector4f ColorDark::orange = Eigen::Vector4f(0.9f, 0.32f, 0.0f, 1.0f);
const Eigen::Vector4f ColorDark::deepOrange = Eigen::Vector4f(0.75f, 0.21f, 0.05f, 1.0f);

const Eigen::Vector4f ColorDark::palette[16] = {
    Eigen::Vector4f(0.72f, 0.11f, 0.11f, 1.0f), Eigen::Vector4f(0.53f, 0.05f, 0.31f, 1.0f),
    Eigen::Vector4f(0.29f, 0.08f, 0.55f, 1.0f), Eigen::Vector4f(0.19f, 0.11f, 0.57f, 1.0f),
    Eigen::Vector4f(0.1f, 0.14f, 0.49f, 1.0f),  Eigen::Vector4f(0.05f, 0.28f, 0.63f, 1.0f),
    Eigen::Vector4f(0.0f, 0.34f, 0.61f, 1.0f),  Eigen::Vector4f(0.0f, 0.38f, 0.39f, 1.0f),
    Eigen::Vector4f(0.0f, 0.3f, 0.25f, 1.0f),   Eigen::Vector4f(0.11f, 0.37f, 0.13f, 1.0f),
    Eigen::Vector4f(0.2f, 0.41f, 0.12f, 1.0f),  Eigen::Vector4f(0.51f, 0.47f, 0.09f, 1.0f),
    Eigen::Vector4f(0.96f, 0.5f, 0.09f, 1.0f),  Eigen::Vector4f(1.0f, 0.44f, 0.0f, 1.0f),
    Eigen::Vector4f(0.9f, 0.32f, 0.0f, 1.0f),   Eigen::Vector4f(0.75f, 0.21f, 0.05f, 1.0f)};

const Eigen::Vector4f ColorLight::red = Eigen::Vector4f(0.9f, 0.45f, 0.45f, 1.0f);
const Eigen::Vector4f ColorLight::pink = Eigen::Vector4f(0.94f, 0.38f, 0.57f, 1.0f);
const Eigen::Vector4f ColorLight::purple = Eigen::Vector4f(0.73f, 0.41f, 0.78f, 1.0f);
const Eigen::Vector4f ColorLight::deepPurple = Eigen::Vector4f(0.58f, 0.46f, 0.8f, 1.0f);
const Eigen::Vector4f ColorLight::indigo = Eigen::Vector4f(0.47f, 0.53f, 0.8f, 1.0f);
const Eigen::Vector4f ColorLight::blue = Eigen::Vector4f(0.39f, 0.71f, 0.96f, 1.0f);
const Eigen::Vector4f ColorLight::lightBlue = Eigen::Vector4f(0.31f, 0.76f, 0.97f, 1.0f);
const Eigen::Vector4f ColorLight::cyan = Eigen::Vector4f(0.3f, 0.82f, 0.88f, 1.0f);
const Eigen::Vector4f ColorLight::teal = Eigen::Vector4f(0.3f, 0.71f, 0.67f, 1.0f);
const Eigen::Vector4f ColorLight::green = Eigen::Vector4f(0.51f, 0.78f, 0.52f, 1.0f);
const Eigen::Vector4f ColorLight::lightGreen = Eigen::Vector4f(0.68f, 0.84f, 0.51f, 1.0f);
const Eigen::Vector4f ColorLight::lime = Eigen::Vector4f(0.86f, 0.91f, 0.46f, 1.0f);
const Eigen::Vector4f ColorLight::yellow = Eigen::Vector4f(1.0f, 0.95f, 0.46f, 1.0f);
const Eigen::Vector4f ColorLight::amber = Eigen::Vector4f(1.0f, 0.84f, 0.31f, 1.0f);
const Eigen::Vector4f ColorLight::orange = Eigen::Vector4f(1.0f, 0.72f, 0.3f, 1.0f);
const Eigen::Vector4f ColorLight::deepOrange = Eigen::Vector4f(1.0f, 0.54f, 0.4f, 1.0f);

const Eigen::Vector4f ColorLight::palette[16] = {
    Eigen::Vector4f(0.9f, 0.45f, 0.45f, 1.0f),  Eigen::Vector4f(0.94f, 0.38f, 0.57f, 1.0f),
    Eigen::Vector4f(0.73f, 0.41f, 0.78f, 1.0f), Eigen::Vector4f(0.58f, 0.46f, 0.8f, 1.0f),
    Eigen::Vector4f(0.47f, 0.53f, 0.8f, 1.0f),  Eigen::Vector4f(0.39f, 0.71f, 0.96f, 1.0f),
    Eigen::Vector4f(0.31f, 0.76f, 0.97f, 1.0f), Eigen::Vector4f(0.3f, 0.82f, 0.88f, 1.0f),
    Eigen::Vector4f(0.3f, 0.71f, 0.67f, 1.0f),  Eigen::Vector4f(0.51f, 0.78f, 0.52f, 1.0f),
    Eigen::Vector4f(0.68f, 0.84f, 0.51f, 1.0f), Eigen::Vector4f(0.86f, 0.91f, 0.46f, 1.0f),
    Eigen::Vector4f(1.0f, 0.95f, 0.46f, 1.0f),  Eigen::Vector4f(1.0f, 0.84f, 0.31f, 1.0f),
    Eigen::Vector4f(1.0f, 0.72f, 0.3f, 1.0f),   Eigen::Vector4f(1.0f, 0.54f, 0.4f, 1.0f)};
} // namespace csVisOpenGL
