#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>  // Для роботи з часом (time())

// Клас Gallows реалізує гру "Виселица"
class Gallows
{
    std::string m_word;               // Оригінальне слово, яке треба відгадати
    std::string m_hiddenWord;         // Слово з прихованими буквами (замість букв – #)
    std::vector<char> m_guessed;      // Масив відгаданих літер
    std::vector<char> m_mistakes;     // Масив неправильних літер
    int m_attempts;                   // Лічильник спроб (помилок)

    // Функція для завантаження списку слів з файлу
    std::vector<std::string> loadWordsFile(const std::string& filename)
    {
        std::ifstream file(filename); // Відкриваємо файл для читання
        std::vector<std::string> words; // Список слів
        if (file) // Якщо файл відкритий успішно
        {
            std::string line;
            // Читаємо кожне слово і додаємо його у список
            while (std::getline(file, line))
            {
                words.push_back(line); // Додаємо слово у вектор
            }
        }
        return words; // Повертаємо список слів
    }

    // Функція для вибору випадкового слова зі списку
    std::string randomWord(const std::vector<std::string>& words)
    {
        return words[std::rand() % words.size()]; // Повертаємо випадкове слово з масиву
    }

    // Функція для виведення поточного стану гри
    void showStatus() const
    {
        std::cout << "Word: ";
        // Для кожної букви слова виводимо або # (якщо ще не відгадано), або саму букву
        for (size_t i = 0; i < m_word.size(); ++i)
        {
            if (m_hiddenWord[i] == '_') // Якщо буква ще не відгадана
            {
                std::cout << "#";  // Виводимо # замість букви
            }
            else
            {
                std::cout << m_hiddenWord[i];  // Виводимо відгадану букву
            }
        }
        std::cout << "\n";
        std::cout << "Attempts: " << m_attempts << "\n"; // Виводимо кількість спроб
        std::cout << "Guessed letters: ";
        for (char c : m_guessed) // Виводимо всі відгадані літери
        {
            std::cout << c << " ";
        }
        std::cout << "\n";
        std::cout << "Incorrect letters: ";
        for (char c : m_mistakes) // Виводимо всі неправильні літери
        {
            std::cout << c << " ";
        }
        std::cout << "\n";
        printGallows(); // Виводимо виселицю в залежності від кількості спроб
    }

    // Функція для виведення виселиці
    void printGallows() const
    {
        std::cout << " _______\n";  // Верхняя часть виселицы
        // Виводимо різні частини виселиці залежно від кількості спроб
        switch (m_attempts)
        {
        case 6:
            std::cout << "  |\n  O\n /|\\\n / \\\n"; break; // Повна виселица
        case 5:
            std::cout << "  |\n  O\n /|\\\n / \n"; break; // Ноги не повні
        case 4:
            std::cout << "  |\n  O\n /|\\\n\n"; break; // Обе руки, без ніг
        case 3:
            std::cout << "  |\n  O\n / \n\n"; break; // Одна рука
        case 2:
            std::cout << "  |\n  O\n\n"; break; // Туловище без рук
        case 1:
            std::cout << "  |\n\n\n\n\n"; break; // Голова
        case 0:
            std::cout << "\n\n\n\n\n\n"; break; // Пусто
        default:
            break;
        }
    }

    // Функція для перевірки, чи відгадано слово
    bool isWordGuessed() const
    {
        return m_hiddenWord == m_word; // Порівнюємо зашифроване слово з оригіналом
    }

public:
    // Конструктор класу Gallows
    Gallows(const std::string& filename)
        : m_attempts(0)
    {
        std::vector<std::string> words = loadWordsFile(filename); // Завантажуємо список слів
        if (!words.empty()) // Якщо в списку є слова
        {
            std::srand(static_cast<unsigned>(std::time(0))); // Ініціалізація генератора випадкових чисел
            m_word = randomWord(words); // Вибір випадкового слова зі списку
            m_hiddenWord = std::string(m_word.size(), '_'); // Ініціалізація схованого слова (усі символи – _)
        }
        else
        {
            std::cerr << "The file is empty or no words found.\n"; // Якщо файл порожній
            exit(1); // Завершення програми у разі помилки
        }
    }

    // Основний процес гри
    void play()
    {
        // Перезапуск гри автоматично
        m_attempts = 0; // Оновлюємо кількість спроб
        m_guessed.clear(); // Очищаємо список відгаданих літер
        m_mistakes.clear(); // Очищаємо список неправильних літер
        m_hiddenWord = std::string(m_word.size(), '_'); // Оновлюємо сховане слово

        // Основний цикл гри
        while (!isWordGuessed() && m_attempts < 6)
        {
            showStatus(); // Виводимо поточний стан гри
            char guess;
            std::cout << "Choose a letter: ";
            std::cin >> guess;

            // Перевірка правильності вводу
            if (!(guess >= 'a' && guess <= 'z') && !(guess >= 'A' && guess <= 'Z'))
            {
                std::cerr << "Invalid input. Please enter a letter.\n";
                continue;
            }

            if (guess >= 'A' && guess <= 'Z')
            {
                guess += 'a' - 'A'; // Перетворюємо на малу літеру, якщо введено велику
            }

            // Перевірка, чи була вже відгадана ця буква
            bool alreadyGuessed = false;
            for (char c : m_guessed)
            {
                if (c == guess)
                {
                    alreadyGuessed = true;
                    break;
                }
            }

            if (alreadyGuessed)
            {
                std::cerr << "You already guessed this letter. Choose another one\n";
                continue;
            }

            m_guessed.push_back(guess); // Додаємо букву до списку відгаданих літер
            bool correctGuess = false;
            // Перевірка, чи є ця буква в слові
            for (size_t i = 0; i < m_word.size(); ++i)
            {
                if (m_word[i] == guess)
                {
                    m_hiddenWord[i] = guess; // Замінюємо _ на відгадану букву
                    correctGuess = true;
                }
            }

            if (!correctGuess) // Якщо буква неправильна
            {
                m_mistakes.push_back(guess); // Додаємо в список помилок
                ++m_attempts; // Збільшуємо кількість спроб
            }
        }

        // Перевірка результату гри
        if (isWordGuessed()) // Якщо слово відгадано
        {
            std::cout << "WOW! You win!\n";
        }
        else
        {
            std::cout << "Game over! The word was: " << m_word << "\n";
        }

        // Виведення всіх літер і результатів гри
        std::cout << "Correct letters: ";
        for (char c : m_guessed)
        {
            if (std::find(m_mistakes.begin(), m_mistakes.end(), c) == m_mistakes.end()) // Якщо буква не є помилкою
                std::cout << c << " ";
        }

        std::cout << "\nIncorrect letters: ";
        for (char c : m_mistakes)
        {
            std::cout << c << " ";
        }
        std::cout << "\n";

        // Запит на продовження гри
        char playAgain;
        std::cout << "Do you want to play again? (y/n): ";
        std::cin >> playAgain;

        if (playAgain == 'y' || playAgain == 'Y')
        {
            // Створюємо нову гру з новим словом
            Gallows newGame("words.txt");
            newGame.play(); // Запускаємо нову гру
        }
    }
};

// Головна функція
int main()
{
    Gallows game("words.txt"); // Створюємо гру з файлом слів
    game.play(); // Запускаємо гру
}