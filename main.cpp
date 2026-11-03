
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "SortingAlgorithms.h"

using namespace std;

enum class Algorithm
{
    Merge,
    Bubble
};

vector<int> generateRandomArray(int n)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(10, 100);

    vector<int> arr(n);

    for (int& value : arr)
        value = dist(gen);

    return arr;
}

bool parseCustomInput(const string& input, vector<int>& result)
{
    vector<int> values;
    stringstream ss(input);
    string token;

    while (getline(ss, token, ','))
    {
        try
        {
            size_t position;
            int value = stoi(token, &position);

            while (position < token.size() &&
                   isspace(static_cast<unsigned char>(token[position])))
            {
                position++;
            }

            if (position != token.size() || value < 1 || value > 100)
                return false;

            values.push_back(value);
        }
        catch (...)
        {
            return false;
        }
    }

    if (values.size() < 2 || values.size() > 30)
        return false;

    result = values;
    return true;
}

void drawButton(
    sf::RenderWindow& window,
    sf::Font& font,
    const string& label,
    float x,
    float y,
    float width,
    sf::Color color
)
{
    sf::RectangleShape button({width, 45.f});
    button.setPosition({x, y});
    button.setFillColor(color);
    button.setOutlineColor(sf::Color(203, 213, 225));
    button.setOutlineThickness(1.f);

    sf::Text text(font, label, 16);
    text.setFillColor(sf::Color::White);
    text.setPosition({x + 12.f, y + 12.f});

    window.draw(button);
    window.draw(text);
}

int main()
{
    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        cout << "Font could not be loaded!" << endl;
        return 1;
    }

    vector<int> original = generateRandomArray(20);
    vector<int> arr = original;

    vector<SortEvent> events;
    vector<bool> sorted(arr.size(), false);

    sf::RenderWindow window(
        sf::VideoMode({1100u, 750u}),
        "ProjectCpp - Sorting Visualizer"
    );

    window.setFramerateLimit(60);

    bool isPlaying = false;
    bool isFinished = false;
    bool inputActive = false;

    int eventIndex = 0;
    int activeA = -1;
    int activeB = -1;

    EventType activeType = EventType::Compare;

    Algorithm selectedAlgorithm = Algorithm::Merge;

    float animationSpeed = 0.5f;

    sf::Clock animationClock;

    string inputText = "45, 12, 89, 23, 10, 67";
    string status = "Ready to Sort";

    // Prepare selected sorting algorithm
    auto prepareSorting = [&]()
    {
        vector<int> workingArray = arr;

        events.clear();
        sorted.assign(arr.size(), false);

        eventIndex = 0;
        activeA = -1;
        activeB = -1;

        if (selectedAlgorithm == Algorithm::Merge)
        {
            mergeSort(
                workingArray,
                0,
                static_cast<int>(workingArray.size()) - 1,
                events
            );
        }
        else
        {
            bubbleSort(workingArray, events);
        }

        isPlaying = false;
        isFinished = false;

        activeType = EventType::Compare;

        status = "Ready to Sort";

        animationClock.restart();
    };

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Keyboard input
            if (const auto* key =
                event->getIf<sf::Event::KeyPressed>())
            {
                if (inputActive &&
                    key->code == sf::Keyboard::Key::Backspace &&
                    !inputText.empty())
                {
                    inputText.pop_back();
                }

                if (inputActive &&
                    key->code == sf::Keyboard::Key::Enter)
                {
                    vector<int> customArray;

                    if (parseCustomInput(inputText, customArray))
                    {
                        original = customArray;
                        arr = original;

                        prepareSorting();

                        status = "Custom Array Applied";
                        inputActive = false;
                    }
                    else
                    {
                        status = "Invalid Input! Use 2-30 values (1-100)";
                    }
                }
            }

            // Text input
            if (const auto* textEvent =
                event->getIf<sf::Event::TextEntered>())
            {
                if (inputActive)
                {
                    char32_t character = textEvent->unicode;

                    if ((character >= U'0' && character <= U'9') ||
                        character == U',' || character == U' ')
                    {
                        if (inputText.size() < 100)
                            inputText += static_cast<char>(character);
                    }
                }
            }

            // Mouse input
            if (const auto* mouse =
                event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse->button == sf::Mouse::Button::Left)
                {
                    float mx = static_cast<float>(mouse->position.x);
                    float my = static_cast<float>(mouse->position.y);

                    inputActive =
                        mx >= 50 && mx <= 780 &&
                        my >= 165 && my <= 210;

                    // Start / Pause
                    if (mx >= 50 && mx <= 180 &&
                        my >= 100 && my <= 145)
                    {
                        if (!isFinished)
                        {
                            isPlaying = !isPlaying;

                            status = isPlaying
                                ? "Sorting in Progress..."
                                : "Paused";
                        }

                        animationClock.restart();
                    }

                    // Reset
                    else if (mx >= 195 && mx <= 325 &&
                             my >= 100 && my <= 145)
                    {
                        arr = original;
                        prepareSorting();
                        status = "Array Reset";
                    }

                    // Increase delay
                    else if (mx >= 340 && mx <= 470 &&
                             my >= 100 && my <= 145)
                    {
                        animationSpeed += 0.1f;

                        if (animationSpeed > 1.5f)
                            animationSpeed = 1.5f;
                    }

                    // Decrease delay
                    else if (mx >= 485 && mx <= 615 &&
                             my >= 100 && my <= 145)
                    {
                        animationSpeed -= 0.1f;

                        if (animationSpeed < 0.05f)
                            animationSpeed = 0.05f;
                    }

                    // Random array
                    else if (mx >= 630 && mx <= 780 &&
                             my >= 100 && my <= 145)
                    {
                        original = generateRandomArray(20);
                        arr = original;

                        prepareSorting();
                        status = "Random Array Generated";
                    }

                    // Select Merge Sort
                    else if (mx >= 800 && mx <= 930 &&
                             my >= 100 && my <= 145)
                    {
                        selectedAlgorithm = Algorithm::Merge;

                        arr = original;
                        prepareSorting();

                        status = "Merge Sort Selected";
                    }

                    // Select Bubble Sort
                    else if (mx >= 940 && mx <= 1070 &&
                             my >= 100 && my <= 145)
                    {
                        selectedAlgorithm = Algorithm::Bubble;

                        arr = original;
                        prepareSorting();

                        status = "Bubble Sort Selected";
                    }

                    // Apply custom input
                    else if (mx >= 900 && mx <= 1030 &&
                             my >= 165 && my <= 210)
                    {
                        vector<int> customArray;

                        if (parseCustomInput(inputText, customArray))
                        {
                            original = customArray;
                            arr = original;

                            prepareSorting();

                            status = "Custom Array Applied";
                            inputActive = false;
                        }
                        else
                        {
                            status = "Invalid Input! Use 2-30 values (1-100)";
                        }
                    }
                }
            }
        }

        // Animation processing
        if (isPlaying && !isFinished &&
            animationClock.getElapsedTime().asSeconds() >= animationSpeed)
        {
            if (eventIndex < static_cast<int>(events.size()))
            {
                const SortEvent& current = events[eventIndex];

                activeA = current.indexA;
                activeB = current.indexB;
                activeType = current.type;

                // Apply Merge Sort overwrite
                if (current.type == EventType::Overwrite)
                {
                    arr[current.indexA] = current.value;
                }

                // Apply Bubble Sort swap
                if (current.type == EventType::Swap)
                {
                    swap(arr[current.indexA], arr[current.indexB]);
                }

                // Mark sorted positions
                if (current.type == EventType::Sorted)
                {
                    sorted[current.indexA] = true;
                }

                eventIndex++;
                animationClock.restart();
            }
            else
            {
                isFinished = true;
                isPlaying = false;
                status = "Sorting Completed!";
            }
        }

        window.clear(sf::Color(248, 250, 252));

        // Heading
        sf::Text heading(font, "ProjectCpp", 32);
        heading.setFillColor(sf::Color(30, 41, 59));
        heading.setPosition({50.f, 20.f});
        window.draw(heading);

        string algorithmName =
            selectedAlgorithm == Algorithm::Merge
            ? "MERGE SORT VISUALIZER"
            : "BUBBLE SORT VISUALIZER";

        sf::Text subtitle(font, algorithmName, 14);
        subtitle.setFillColor(sf::Color(100, 116, 139));
        subtitle.setPosition({55.f, 60.f});
        window.draw(subtitle);

        // Main controls
        drawButton(
            window, font, isPlaying ? "Pause" : "Start",
            50, 100, 130, sf::Color(37, 99, 235)
        );

        drawButton(
            window, font, "Reset",
            195, 100, 130, sf::Color(71, 85, 105)
        );

        drawButton(
            window, font, "Speed -",
            340, 100, 130, sf::Color(71, 85, 105)
        );

        drawButton(
            window, font, "Speed +",
            485, 100, 130, sf::Color(71, 85, 105)
        );

        drawButton(
            window, font, "Random",
            630, 100, 150, sf::Color(16, 145, 129)
        );

        // Algorithm selection buttons
        drawButton(
            window, font, "Merge",
            800, 100, 130,
            selectedAlgorithm == Algorithm::Merge
                ? sf::Color(37, 99, 235)
                : sf::Color(71, 85, 105)
        );

        drawButton(
            window, font, "Bubble",
            940, 100, 130,
            selectedAlgorithm == Algorithm::Bubble
                ? sf::Color(37, 99, 235)
                : sf::Color(71, 85, 105)
        );

        // Input box
        sf::RectangleShape inputBox({730.f, 45.f});
        inputBox.setPosition({50.f, 165.f});
        inputBox.setFillColor(sf::Color::White);

        inputBox.setOutlineColor(
            inputActive ? sf::Color(37, 99, 235)
                        : sf::Color(203, 213, 225)
        );

        inputBox.setOutlineThickness(2.f);
        window.draw(inputBox);

        sf::Text inputLabel(
            font,
            inputText.empty() ? "Enter comma-separated numbers" : inputText,
            17
        );

        inputLabel.setFillColor(sf::Color(30, 41, 59));
        inputLabel.setPosition({65.f, 177.f});
        window.draw(inputLabel);

        drawButton(
            window, font, "Apply Input",
            900, 165, 130, sf::Color(37, 99, 235)
        );

        // Status
        sf::Text statusText(font, status, 17);
        statusText.setFillColor(sf::Color(30, 41, 59));
        statusText.setPosition({50.f, 230.f});
        window.draw(statusText);

        sf::Text speedText(
            font,
            "Delay: " + to_string(animationSpeed).substr(0, 4) + " sec",
            15
        );

        speedText.setFillColor(sf::Color(71, 85, 105));
        speedText.setPosition({850.f, 230.f});
        window.draw(speedText);

        // Sorting visualization
        int n = static_cast<int>(arr.size());

        float availableWidth = 900.f;
        float gap = (n <= 10) ? 18.f : 8.f;

        float barWidth = min(
            55.f,
            (availableWidth - (n - 1) * gap) / n
        );

        float totalWidth = n * barWidth + (n - 1) * gap;
        float startX = (1100.f - totalWidth) / 2.f;

        float baseline = 625.f;
        float maxHeight = 250.f;

        for (int i = 0; i < n; i++)
        {
            float height = (arr[i] / 100.f) * maxHeight;

            float x = startX + i * (barWidth + gap);
            float y = baseline - height;

            sf::RectangleShape bar({barWidth, height});
            bar.setPosition({x, y});

            sf::Color barColor(59, 130, 246);

            if (sorted[i])
            {
                barColor = sf::Color(34, 197, 94);
            }
            else if ((activeType == EventType::Compare ||
                      activeType == EventType::Swap) &&
                     (i == activeA || i == activeB))
            {
                barColor = sf::Color(245, 158, 11);
            }
            else if (activeType == EventType::Overwrite &&
                     i == activeA)
            {
                barColor = sf::Color(239, 68, 68);
            }

            bar.setFillColor(barColor);
            window.draw(bar);

            // Value labels
            if (n <= 20)
            {
                sf::Text valueText(
                    font,
                    to_string(arr[i]),
                    14
                );

                valueText.setFillColor(sf::Color(30, 41, 59));

                float labelX = x + (barWidth -
                    valueText.getLocalBounds().size.x) / 2.f;

                valueText.setPosition({labelX, y - 23.f});

                window.draw(valueText);
            }
        }

        // Legend
        sf::Text legend(
            font,
            "Blue: Normal   Yellow: Compare/Swap   Red: Overwrite   Green: Sorted",
            15
        );

        legend.setFillColor(sf::Color(51, 65, 85));
        legend.setPosition({180.f, 680.f});
        window.draw(legend);

        window.display();
    }

    return 0;
}
