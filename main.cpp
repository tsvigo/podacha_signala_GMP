 #include <QApplication>

// int main(int argc, char *argv[])
// {
//     QCoreApplication a(argc, argv);

//     return a.exec();
// }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QImage>
#include <gmpxx.h>
#include <iostream>
#include <vector>
#include <QFileDialog>
//const
    QString BMP_FILE_NAME = "image.bmp"; // название надо брать из диалога выбора файла
const QString RANDOM_NUMBERS_FILE_NAME = "/home/viktor/my_projects_qt_2/sgenerirovaty_neyroni_GMP/random_numbers.bin";
//const
//QString RESULT_FILE_NAME = "result_vector.bin"; // результат склеиваем
//const
//  QString    RESULT_FILE_NAME=BMP_FILE_NAME+"_result_vector.bin";
// /home/viktor/1_rukoy_GMP/Sprite-0001-m.bmp
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<mpz_class> readBMPFile(const QString& fileName) {
    QImage image(fileName);
    if (image.isNull()) {
        std::cerr << "Failed to load BMP file" << std::endl;
        return {};
    }
    std::vector<mpz_class> result;
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);
            QString colorString = QString("%1%2%3").arg(color.red(), 3, 10, QChar('0'))
                                      .arg(color.green(), 3, 10, QChar('0'))
                                      .arg(color.blue(), 3, 10, QChar('0'));
            result.push_back(mpz_class(colorString.toStdString()));
        }
    }
    return result;
}

std::vector<mpz_class> readRandomNumbersFile(const QString& fileName) {
    std::vector<mpz_class> numbers;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Failed to open random numbers file" << std::endl;
        return numbers;
    }
    QDataStream in(&file);
    while (!in.atEnd()) {
        QByteArray byteArray;
        in >> byteArray;
        numbers.push_back(mpz_class(byteArray.constData()));
    }
    file.close();
    return numbers;
}

void writeResultToFile(const QString& fileName, const std::vector<mpz_class>& numbers) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        std::cerr << "Failed to open result file for writing" << std::endl;
        return;
    }
    QDataStream out(&file);
    for (const auto& number : numbers) {
        QByteArray byteArray = number.get_str().c_str();
        out << byteArray;
    }
    file.close();
}

void printVector(const std::vector<mpz_class>& vec) {
    int i=0;
    for (const auto& number : vec) {
        std::cout<<i<<":" << number.get_str() << std::endl;
        i++;
    }
}

int main(int argc, char *argv[]) {
  //  QCoreApplication a(argc, argv);
    // Используем QApplication вместо QCoreApplication, чтобы поддерживать графические компоненты
    QApplication app(argc, argv);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Вызов диалога выбора файла
    BMP_FILE_NAME = QFileDialog::getOpenFileName(nullptr, "Выберите файл",
                                    "/home/viktor/1_rukoy_GMP/"
                                                            ,  "bmp Files (*.bmp)");

    QString
        RESULT_FILE_NAME=BMP_FILE_NAME+"_result_vector.bin";

    // Проверка, был ли файл выбран
    if (!BMP_FILE_NAME.isEmpty()) {
        qDebug() << "Выбранный файл:" << BMP_FILE_NAME;
    } else {
        qDebug() << "Файл не был выбран.";
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Чтение BMP файла и преобразование в вектор mpz_class
    std::vector<mpz_class> bmpVector = readBMPFile(BMP_FILE_NAME);

    // Чтение файла с 205 случайными числами и преобразование в вектор mpz_class
    std::vector<mpz_class> randomVector = readRandomNumbersFile(RANDOM_NUMBERS_FILE_NAME);

    // Формирование общего вектора
    std::vector<mpz_class> resultVector;
    resultVector.insert(resultVector.end(), bmpVector.begin(), bmpVector.begin() + 100);
    resultVector.insert(resultVector.end(), randomVector.begin(),randomVector.begin() + 105
                  //      randomVector.end()
                        );

    // Вывод результирующего вектора в консоль
    printVector(resultVector);

    // Запись результирующего вектора в бинарный файл
    writeResultToFile(RESULT_FILE_NAME, resultVector);

    qDebug() << "RESULT_FILE_NAME: "<<RESULT_FILE_NAME;
    qDebug() <<"resultVector.size()="<<resultVector.size();
    return app.exec();
}

