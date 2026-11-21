#include <csVisOpenGL/painters/PainterCheckerBoard.hpp>

#include <csVisOpenGL/Colors.hpp>
#include <csVisOpenGL/painters/PainterAxes.hpp>
#include <csVisOpenGL/renderers/UniformMeshRenderer.hpp>

namespace csVisOpenGL {

PainterCheckerBoard::PainterCheckerBoard()
    : m_blackSquares(new UniformMeshRenderer()), m_whiteSquares(new UniformMeshRenderer()), m_axes(new PainterAxes()),
      m_paintChecherboard(false), m_paintAxes(false) {}

PainterCheckerBoard::~PainterCheckerBoard() {}

void PainterCheckerBoard::initialize(ShaderFactory &shaderFactory,
                                     std::shared_ptr<QOpenGLExtraFunctions> const &glExtraFunctions) {

  m_blackSquares->initialize(shaderFactory, glExtraFunctions);
  m_blackSquares->setUniformColor(Color::black);
  m_blackSquares->setTriangles(Eigen::Matrix3Xf());

  m_whiteSquares->initialize(shaderFactory, glExtraFunctions);
  m_whiteSquares->setUniformColor(Color::white);
  m_whiteSquares->setTriangles(Eigen::Matrix3Xf());

  m_axes->initialize(shaderFactory, glExtraFunctions);
}

void PainterCheckerBoard::draw(const Camera &camera) {
  if (m_paintChecherboard) {
    m_blackSquares->draw(camera);
    m_whiteSquares->draw(camera);
  }
  if (m_paintAxes) {
    m_axes->draw(camera);
  }
}

void PainterCheckerBoard::setCheckerBoard(int w, int h, float squareSize, const Eigen::Isometry3f &pose, float xBase,
                                          float yBase, float axesScale) {
  m_paintChecherboard = true;

  {
    Eigen::Matrix3Xf baseSquare(3, 6);
    baseSquare.col(0) = Eigen::Vector3f(0, 0, 0);
    baseSquare.col(1) = Eigen::Vector3f(squareSize, 0, 0);
    baseSquare.col(2) = Eigen::Vector3f(squareSize, squareSize, 0);
    baseSquare.col(3) = baseSquare.col(2);
    baseSquare.col(4) = Eigen::Vector3f(0, squareSize, 0);
    baseSquare.col(5) = baseSquare.col(0);

    int numEvenLines = std::ceil(w / 2.0);
    int numOddLines = w - numEvenLines;

    int numSquaresEven = std::ceil(h / 2.0);
    int numSquaresOdd = h - numSquaresEven;

    int numSqBlack = numEvenLines * numSquaresEven + numOddLines * numSquaresOdd;
    int numSqWhite = numEvenLines * numSquaresOdd + numOddLines * numSquaresEven;

    Eigen::Matrix3Xf squaresBlack = Eigen::Matrix3Xf::Zero(3, baseSquare.cols() * numSqBlack);
    Eigen::Matrix3Xf squaresWhite = Eigen::Matrix3Xf::Zero(3, baseSquare.cols() * numSqWhite);
    int countB = 0, countW = 0;
    for (int i = 0; i < numEvenLines; i++) {
      for (int j = 0; j < numSquaresEven; j++) {
        squaresBlack.block(0, countB * baseSquare.cols(), 3, 6) =
            baseSquare.colwise() + Eigen::Vector3f(xBase + j * 2 * squareSize, yBase + i * 2 * squareSize, 0);
        countB++;
      }
      for (int j = 0; j < numSquaresOdd; j++) {
        squaresWhite.block(0, countW * baseSquare.cols(), 3, 6) =
            baseSquare.colwise() + Eigen::Vector3f(xBase + ((j * 2) + 1) * squareSize, yBase + i * 2 * squareSize, 0);
        countW++;
      }
    }
    for (int i = 0; i < numOddLines; i++) {
      for (int j = 0; j < numSquaresOdd; j++) {
        squaresBlack.block(0, countB * baseSquare.cols(), 3, 6) =
            baseSquare.colwise() +
            Eigen::Vector3f(xBase + ((j * 2) + 1) * squareSize, yBase + ((i * 2) + 1) * squareSize, 0);
        countB++;
      }
      for (int j = 0; j < numSquaresEven; j++) {
        squaresWhite.block(0, countW * baseSquare.cols(), 3, 6) =
            baseSquare.colwise() + Eigen::Vector3f(xBase + j * 2 * squareSize, yBase + ((i * 2) + 1) * squareSize, 0);
        countW++;
      }
    }

    squaresBlack.row(2).setConstant(0);
    squaresWhite.row(2).setConstant(0);

    m_blackSquares->setTriangles(pose * squaresBlack);
    m_whiteSquares->setTriangles(pose * squaresWhite);
  }

  if (axesScale > 0) {
    m_paintAxes = true;
    m_axes->setPose(pose, axesScale);
  }
}

} // namespace csVisOpenGL
