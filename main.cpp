#include "biliardo.hpp"
#include "biliardo_statistica.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

int main() {

  try {
    std::cout << "If you want to throw a single ball type 'graphic', if you "
                 "want to run a statistical simulation type 'stat' \n";
    std::string mode_choice;
    std::cin >> mode_choice;

    //STATISTIC SIMULATION

    if (mode_choice == "stat") {
      int N;
      double mu_y0;
      double sigma_y0;
      double mu_th0;
      double sigma_th0;
      double r1;
      double r2;
      double L;

      std::cout << "Number of throws: ";
      std::cin >> N;
      std::cout << "Mean y_0: ";
      std::cin >> mu_y0;
      std::cout << "Standard deviation y_0: ";
      std::cin >> sigma_y0;
      if (sigma_y0 <= 0) {
        throw std::runtime_error("Standard Deviation must be > 0  ");
      }
      std::cout << "Mean theta_0 (radians): ";
      std::cin >> mu_th0;
      std::cout << "Standard deviation theta_0: ";
      std::cin >> sigma_th0;
      if (sigma_th0 <= 0) {
        throw std::runtime_error("Standard Deviation must be > 0  ");
      }
      std::cout << "Ordinate of the left end of the border: ";
      std::cin >> r1;
      std::cout << "Ordinate of the right end of the border: ";
      std::cin >> r2;
      std::cout << "Abscissa of the right end of the border: ";
      std::cin >> L;

      pf::Border b1(r1, r2, L);
      pf::Border b2(-r1, -r2, L);

      pf::StatsResult res =
          simulate_stats(N, mu_y0, sigma_y0, mu_th0, sigma_th0, b1, b2, L);

      std::cout << "\n \n == Statistic results == \n \n";
      std::cout << "Number Number of valid simulations: " << res.success_count
                << "/" << N << "\n";
      std::cout << "Mean y_f: " << res.mean_yf << "\n";
      std::cout << "Standard devaition y_f: " << res.stdev_yf << "\n";
      std::cout << "Symmetry coefficient y_f: " << res.coeff_simm_yf << "\n";
      std::cout << "Flattening coefficient y_f: " << res.coeff_app_yf << "\n";
      std::cout << "Mean theta_f: " << res.mean_thf << "\n";
      std::cout << "Standard deviation theta_f: " << res.stdev_thf << "\n";
      std::cout << "Symmetry coefficient theta_f: " << res.coeff_simm_thf
                << "\n";
      std::cout << "Flattening coefficient theta_f: " << res.coeff_app_thf
                << "\n";

      return 0;
    }

    //SINGLE THROW

    // preparation of the graphic tools (??)
    sf::RenderWindow window(sf::VideoMode(800, 600), "Biliardo triangolare",
                            sf::Style::Default);
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
      std::cerr << "error: font can't be found";
      return -1;
    }
    sf::Clock clock;
    sf::Time update = sf::seconds(0.016f);
    int step = 0;
    bool finished = false;
    std::string answer;

    // graphic ball
    sf::CircleShape circle;
    circle.setRadius(7.f);
    circle.setOrigin(7.f, 7.f);
    circle.setPosition(sf::Vector2f(0.f, 300.f));
    circle.setFillColor(sf::Color::Yellow);

    // borders
    std::array<sf::Vertex, 2> border1 = {
        sf::Vertex{sf::Vector2f(0.f, 250.f)},
        sf::Vertex{sf::Vector2f(800.f, 250.f)}};
    border1[0].color = sf::Color::Green;
    border1[1].color = sf::Color::Green;

    std::array<sf::Vertex, 2> border2 = {
        sf::Vertex{sf::Vector2f(0.f, 350.f)},
        sf::Vertex{sf::Vector2f(800.f, 350.f)}};
    border2[0].color = sf::Color::Green;
    border2[1].color = sf::Color::Green;

    // logic objects
    pf::Ball ball({0, 0}, 0);
    pf::Border b1(0, 0, 1);
    pf::Border b2(0, 0, 1);
    double y, r1, r2, l;

    // x-axis
    std::array<sf::Vertex, 2> middle_line = {
        sf::Vertex{sf::Vector2f(0.f, 300.f)},
        sf::Vertex{sf::Vector2f(800.f, 300.f)}};
    middle_line[0].color = sf::Color::White;
    middle_line[1].color = sf::Color::White;

    std::array<sf::Vertex, 2> arrow_up = {
        sf::Vertex{sf::Vector2f(792, 292.f)},
        sf::Vertex{sf::Vector2f(800.f, 300.f)}};
    middle_line[0].color = sf::Color::White;
    middle_line[1].color = sf::Color::White;

    std::array<sf::Vertex, 2> arrow_down = {
        sf::Vertex{sf::Vector2f(792, 308.f)},
        sf::Vertex{sf::Vector2f(800.f, 300.f)}};
    middle_line[0].color = sf::Color::White;
    middle_line[1].color = sf::Color::White;

    // TEXTS TO VISUALIZE on the graphic window
    // questions
    sf::Text questions("", font, 15);
    questions.setFillColor(sf::Color::White);
    questions.setPosition(50.f, 50.f);

    // answer (input)
    sf::Text inputText("", font, 15);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(50.f, 120.f);
    sf::Text answer_("Answer:", font, 15);
    answer_.setFillColor(sf::Color::White);
    answer_.setPosition(50.f, 110.f);

    // answer (output)
    sf::Text response("", font, 15);
    response.setFillColor(sf::Color::Green);
    response.setPosition(50.f, 200.f);

    std::vector<std::string> questionText = {
        "Please insert the ordinate of the ball:\nGraphic window's range: [-300,300]\n",
        "Please, insert the launch angle (radians):\nAllowed values: [-1.55, 1.55]",
        "Please, insert the ordinate of the left end of the border:\nGraphic window's range [-300,300].\nAny negative value will be assigned to the bottom border.\n",
        "Please, insert the abscissa of the right end of the border:\nGraphic window's range [0,800]\n ",
        "Please, insert the ordinate of the right end of the border:\nGraphic window's range [-300,300].\nAny negative value will be assigned to the bottom border \n",
        "Press SPACE to throw the ball.\n"};

    auto updateQuestion = [&](int s) {
      if (s >= 0 && static_cast<size_t>(s) < questionText.size())
        questions.setString(questionText[static_cast<size_t>(s)]);
      else
        questions.setString("");
    };

    updateQuestion(step);
    questions.setCharacterSize(15);
    questions.setPosition(50.f, 50.f);
    std::string userInput;

    // handling the graphic window
    while (window.isOpen()) {
      sf::Event event;
      sf::Text result_text;

      if (clock.getElapsedTime() >= update) {
        window.clear();
        window.draw(circle);
        window.draw(middle_line.data(), middle_line.size(),
                    sf::PrimitiveType::Lines);
        window.draw(arrow_up.data(), arrow_up.size(), sf::PrimitiveType::Lines);
        window.draw(arrow_down.data(), arrow_down.size(),
                    sf::PrimitiveType::Lines);
        window.draw(border1.data(), border1.size(), sf::PrimitiveType::Lines);
        window.draw(border2.data(), border2.size(), sf::PrimitiveType::Lines);
        window.draw(questions);
        window.draw(response);
        window.draw(answer_);
        window.draw(inputText);
        window.display();
        clock.restart();
      }

      while (window.pollEvent(event)) {

        // Close window-> End Program
        if (event.type == sf::Event::Closed) {
          window.close();
          std::cerr
              << "Programma terminato: la finestra grafica è stata chiusa.\n";
          return -1;
        }

        if (!finished && event.type == sf::Event::TextEntered) {
          if (event.text.unicode == 8 && !userInput.empty()) {
            // Backspace
            userInput.erase(userInput.size() - 1, 1);
          } else if (event.text.unicode == 13) {
            // Enter: saves the answer and prints the next question
            answer = userInput;
            userInput.clear();
            step++;

            if (step == 1) {
              y = std::stod(answer);
              ball.move_to({0, y + 300});

              circle.setPosition({0.f, static_cast<float>(300.f - y)});
            }

            if (step == 2) {
              double d = std::stod(answer);
              ball.set_angle(d);
            }

            if (step == 3) {
              r1 = std::stod(answer);

              if (r1 >= 0) {
                b1.set_r1(300 + r1);
                b2.set_r1(300 - r1);
                border1[0].position.y = static_cast<float>(300.f - r1);
                border2[0].position.y = static_cast<float>(300.f + r1);
              } else {
                b2.set_r1(300 + r1);
                b1.set_r1(300 - r1);
                border2[0].position.y = static_cast<float>(300.f - r1);
                border1[0].position.y = static_cast<float>(300.f + r1);
              }
            }

            if (step == 4) {
              l = std::stod(answer);
              b1.set_L(l);
              b2.set_L(l);
              border2[1].position.x = static_cast<float>(l);
              border1[1].position.x = static_cast<float>(l);
            }

            if (step == 5) {
              r2 = std::stod(answer);
              if (r2 >= 0) {
                b1.set_r2(300 + r2);
                b2.set_r2(300 - r2);
                border1[1].position.y = static_cast<float>(300.f - r2);
                border2[1].position.y = static_cast<float>(300.f + r2);

              } else {
                b2.set_r2(300 + r2);
                b1.set_r2(300 - r2);
                border2[1].position.y = static_cast<float>(300.f - r2);
                border1[1].position.y = static_cast<float>(300.f + r2);
              }

              b1.set_slopeup((b1.r2() - b1.r1()) / b1.L());
              b2.set_slopeup((b2.r2() - b2.r1()) / b2.L());
            }

          } else if (event.text.unicode >= 32) {
            userInput += static_cast<char>(event.text.unicode);
          }

          inputText.setString(userInput);
          inputText.setFillColor(sf::Color::White);
          inputText.setPosition(120.f, 111.f);
          inputText.setCharacterSize(15);

          updateQuestion(step);
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Key::Space && step == 5) {

          pf::Border::initial_checks(b1, b2, ball);
          finished = true;

          double y_ = ball.coordba().y;
          double x_ = ball.coordba().x;

          pf::Result end = pf::Result::BallSimulation(b1, b2, ball);

          double x_end = end.result.coordba().x;
          double y_end = end.result.coordba().y - 300;
          double d_end = end.result.d();

          for (size_t position = 0; position < end.trajectory.size();
               ++position) {
            circle.move(
                static_cast<float>(end.trajectory[position].coordba().x - x_),
                static_cast<float>(
                    -(end.trajectory[position].coordba().y - y_)));
            x_ = end.trajectory[position].coordba().x;
            y_ = end.trajectory[position].coordba().y;
            window.clear();
            window.draw(circle);
            window.draw(middle_line.data(), middle_line.size(),
                        sf::PrimitiveType::Lines);
            window.draw(arrow_up.data(), arrow_up.size(),
                        sf::PrimitiveType::Lines);
            window.draw(arrow_down.data(), arrow_down.size(),
                        sf::PrimitiveType::Lines);
            window.draw(border1.data(), border1.size(),
                        sf::PrimitiveType::Lines);
            window.draw(border2.data(), border2.size(),
                        sf::PrimitiveType::Lines);

            window.draw(response);
            window.display();
            sf::sleep(sf::milliseconds(64));
          }

          std::ostringstream oss;
          oss << "The final position of the ball is: (" << x_end << "," << y_end
              << ") with a degree of " << d_end
              << " radians.\n The ball executed " << end.bounces
              << " bounces\n";
          response.setCharacterSize(15);
          response.setFillColor(sf::Color::Green);
          response.setPosition(30, 550);
          response.setString(oss.str());
        }

        
      }
    }
  } catch (std::exception const &e) {
    std::cerr << "Exception captured : '" << e.what() << "'\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
  }
}