
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_tostring.hpp>
#include <ostream>
#include <sstream>

#include "tdl/Matrix/PixelMatrix.hpp"

TEST_CASE("PixelMatrix: Constructor with size", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix(tdl::Vector2u(10, 10));
  CHECK(matrix.getSize() == tdl::Vector2u(10, 10));
}

TEST_CASE("PixelMatrix: Constructor without size", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix;
  CHECK(matrix.getSize() == tdl::Vector2u(0, 0));
}

TEST_CASE("PixelMatrix: Copy constructor", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix(tdl::Vector2u(10, 10));
  tdl::PixelMatrix copy(matrix);
  CHECK(copy.getSize() == tdl::Vector2u(10, 10));
}

TEST_CASE("PixelMatrix: Move constructor", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix(tdl::Vector2u(10, 10));
  tdl::PixelMatrix copy(std::move(matrix));
  CHECK(copy.getSize() == tdl::Vector2u(10, 10));
}

TEST_CASE("PixelMatrix: Copy assignment", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix(tdl::Vector2u(10, 10));
  tdl::PixelMatrix copy;
  copy = matrix;
  CHECK(copy.getSize() == tdl::Vector2u(10, 10));
}

TEST_CASE("PixelMatrix: Move assignment", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix(tdl::Vector2u(10, 10));
  tdl::PixelMatrix copy;
  copy = std::move(matrix);
  CHECK(copy.getSize() == tdl::Vector2u(10, 10));
}

TEST_CASE("PixelMatrix: Get pixel", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix(tdl::Vector2u(10, 10));
  tdl::Pixel pixel = matrix.getPixel(tdl::Vector2u(1, 1));
  CHECK(pixel == tdl::Pixel(0, 0, 0, 255));
}

TEST_CASE("PixelMatrix: GetPixel out of sier", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix(tdl::Vector2u(10, 10));
  tdl::Pixel pixel = matrix.getPixel(tdl::Vector2u(15, 15));
  CHECK(pixel == matrix.getPixel(tdl::Vector2u(0, 0)));
}

TEST_CASE("PixelMatrix: Set pixel", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix(tdl::Vector2u(10, 10));
  tdl::Pixel pixel(255, 255, 255, 255);
  matrix.setPixel(tdl::Vector2u(5, 5), pixel);
  CHECK(matrix.getPixel(tdl::Vector2u(5, 5)) == tdl::Pixel(255, 255, 255, 255));
}

TEST_CASE("PixelMatrix: registerToUpdate", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix(tdl::Vector2u(10, 10));
  matrix.registerToUpdate(tdl::Vector2u(5, 5));
  CHECK(matrix.getToUpdate().front() == tdl::Vector2u(5, 5));
}

TEST_CASE("PixelMatrix : operator-()", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix(tdl::Vector2u(10, 10));
  std::optional<tdl::RectU> rect;
  rect = tdl::RectU(0, 0, 5, 5);
  tdl::PixelMatrix copy = matrix - rect;
  CHECK(copy.getSize() == tdl::Vector2u(5, 5));
}

TEST_CASE("PixelMatrix : operator()", "[Matrix][PixelMatrix]") {
  tdl::PixelMatrix matrix(tdl::Vector2u(10, 10));
  std::optional<tdl::RectU> rect;
  matrix = matrix - rect;
  CHECK(matrix.getSize() == tdl::Vector2u(10, 10));
}