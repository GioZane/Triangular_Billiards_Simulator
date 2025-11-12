#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "biliardo.hpp"
#include "doctest.h"

TEST_CASE("Testing BallSimulation") {
  pf::Ball ball({0, 0}, 0);
  pf::Border b1(0, 0, 1);
  pf::Border b2(0, 0, 1);
  pf::Result end(0, ball);

  SUBCASE("2 bounces") {
    ball.move_to({0, 300});
    ball.set_angle(0.785398);
    b1.move_border(550, 550, 800);
    b2.move_border(50, 50, 800);
    end = pf::Result::BallSimulation(b1, b2, ball);
    CHECK(end.result.d() == doctest::Approx(0.785398).epsilon(0.05));
    CHECK(end.result.coordba().y == doctest::Approx(100.).epsilon(0.05));
    CHECK(end.bounces == 2);
  }

  SUBCASE("Ball comes back") {
    ball.move_to({0, 310});
    ball.set_angle(1.45);
    b1.move_border(400, 308, 15);//y1,x2,y2
    b2.move_border(200, 292, 15);
    CHECK_THROWS(pf::Result::BallSimulation(b1, b2, ball));
  }

  
  SUBCASE("Ball enters correclty but then comes back") {
    ball.move_to({0, 310});
    ball.set_angle(0.524);
    b1.move_border(320, 308, 50);
    b2.move_border(280, 292, 50);
    CHECK_THROWS(pf::Result::BallSimulation(b1, b2, ball));
  }
    
  
  SUBCASE("Ball comes back p2") {
    ball.move_to({0, 320});
    ball.set_angle(1.220);
    b1.move_border(400, 380, 70);
    b2.move_border(200, 220, 70);
    CHECK_THROWS(pf::Result::BallSimulation(b1, b2, ball));
  }

  SUBCASE("Ball comes out without touching the borders") {
    ball.move_to({0, 310});
    ball.set_angle(0.524);
    b1.move_border(400, 385, 40);
    b2.move_border(200, 215, 40);
    pf::Result res = pf::Result::BallSimulation(b1, b2, ball);
    CHECK(res.result.coordba().y == doctest::Approx(333.115).epsilon(0.05));
    CHECK(res.result.d() == doctest::Approx(0.524).epsilon(0.05));
    CHECK(res.bounces == 0);
  }

  SUBCASE("Ball comes out after one bounce") {
    ball.move_to({0, 340});
    ball.set_angle(0.785);
    b1.move_border(400, 385, 50);
    b2.move_border(200, 215, 50);
    pf::Result res = pf::Result::BallSimulation(b1, b2, ball);
    CHECK(res.result.coordba().y == doctest::Approx(367.586).epsilon(0.05));
    CHECK(res.bounces == 1);
    CHECK(res.result.d() == doctest::Approx(-1.3679).epsilon(0.05));
  }

  SUBCASE("Ball comes out after 2 bounces") {
    ball.move_to({0, 330});
    ball.set_angle(-1.100);
    b1.move_border(380, 370, 98);
    b2.move_border(220, 230, 98);
    pf::Result res = pf::Result::BallSimulation(b1, b2, ball);
    CHECK(res.result.coordba().y == doctest::Approx(292.111).epsilon(0.05));
    CHECK(res.bounces == 2);
    CHECK(res.result.d() == doctest::Approx(-1.50675).epsilon(0.05)); //prima era -1.50675
  }


  SUBCASE("r2>r1") {
    ball.move_to({0, 350});
    ball.set_angle(1.0472);
    b1.move_border(370, 390, 60);
    b2.move_border(230, 210, 60);
    pf::Result res = pf::Result::BallSimulation(b1, b2, ball);
    CHECK(res.result.coordba().y == doctest::Approx(349.30).epsilon(0.05));
    CHECK(res.result.d() == doctest::Approx(-0.4037).epsilon(0.05));
  }

  SUBCASE("r2>r1 and negative angle") {
    ball.move_to({0, 290});
    ball.set_angle(-1.120);
    b1.move_border(370, 390, 60);
    b2.move_border(230, 210, 60);
    pf::Result res = pf::Result::BallSimulation(b1, b2, ball);
    CHECK(res.result.coordba().y == doctest::Approx(231.55).epsilon(0.05));
    CHECK(res.result.d() == doctest::Approx(0.4765).epsilon(0.05));
  }

  SUBCASE("r2>r1, negative angle and the slope remains negative") {
    ball.move_to({0, 290});
    ball.set_angle(-1.25);
    b1.move_border(370, 450, 60);
    b2.move_border(230, 150, 60);
    pf::Result res = pf::Result::BallSimulation(b1, b2, ball);
      CHECK(res.result.d()<0);
  }

  SUBCASE("r1>r2, positive angle and the slope remains positive") {
    ball.move_to({0, 290});
    ball.set_angle(1.3);
    b1.move_border(365, 450, 67);
    b2.move_border(225, 150, 67);
    pf::Result res = pf::Result::BallSimulation(b1, b2, ball);
      CHECK(res.result.d()>0);
  }
}

TEST_CASE("Testing initial_checks") {

  pf::Ball ball({0, 0}, 0);
  pf::Border b1(0, 0, 1);
  pf::Border b2(0, 0, 1);
  pf::Result end(0, ball);

  SUBCASE("Not accepetd angle >90°") {
    ball.set_angle(1.57);
    CHECK_THROWS(pf::Border::initial_checks(b1, b2, ball));
  }

  SUBCASE("Ball outside borders") {
    ball.move_to({0, 578});
    b1.move_border(480, 580, 1409);
    b2.move_border(120, 20, 1409);
    CHECK_THROWS(pf::Border::initial_checks(b1, b2, ball));
  }

  SUBCASE("Negative L") {
    ball.move_to({0, 578});
    b1.move_border(480, 580, -50);
    b2.move_border(120, 20, -50);
    CHECK_THROWS(pf::Border::initial_checks(b1, b2, ball));
  }

  SUBCASE("Crossed borders") {
    ball.move_to({0, 347});
    b1.move_border(560, 300, 1400);
    b2.move_border(40, 300, 1400);
    CHECK_THROWS(pf::Border::initial_checks(b1, b2, ball));
  }

  SUBCASE("Vertical borders obstruct ball") {
    ball.move_to({0, 460});
    b1.move_border(570, 305, 0);
    b2.move_border(30, 295, 0);
    CHECK_THROWS(pf::Border::initial_checks(b1, b2, ball));
  }
}