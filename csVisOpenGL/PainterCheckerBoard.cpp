#include "PainterCheckerBoard.h"

#include "Colors.h"
#include "PainterAxes.h"
#include "UniformMeshRenderer.h"

namespace csVisOpenGL {

PainterCheckerBoard::PainterCheckerBoard()
    : blackSquares(new UniformMeshRenderer())
    , whiteSquares(new UniformMeshRenderer())
    , axes(new PainterAxes())
    , paintChecherboard(false)
    , paintAxes(false) {}

PainterCheckerBoard::~PainterCheckerBoard() {}

void PainterCheckerBoard::initialize(ShaderFactory* shaderFactory) {

  blackSquares->initialize(shaderFactory);
  blackSquares->setUniformColor(Color::black);
  blackSquares->setTriangles(Eigen::Matrix3Xf());

  whiteSquares->initialize(shaderFactory);
  whiteSquares->setUniformColor(Color::white);
  whiteSquares->setTriangles(Eigen::Matrix3Xf());

  axes->initialize(shaderFactory);
}

void PainterCheckerBoard::draw(const Camera& camera) {
  if (paintChecherboard) {
    blackSquares->draw(camera);
    whiteSquares->draw(camera);
  }
  if (paintAxes) {
    axes->draw(camera);
  }
}

void PainterCheckerBoard::setCheckerBoard(int w, int h, float squareSize, const Eigen::Isometry3f& pose, float xBase, float yBase,
                                          float axesScale) {
  paintChecherboard = true;

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
            baseSquare.colwise() + Eigen::Vector3f(xBase + ((j * 2) + 1) * squareSize, yBase + ((i * 2) + 1) * squareSize, 0);
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

    blackSquares->setTriangles(pose * squaresBlack);
    whiteSquares->setTriangles(pose * squaresWhite);
  }

  if (axesScale > 0) {
    paintAxes = true;
    axes->setPose(pose, axesScale);
  }
}

} // namespace csVisOpenGL
