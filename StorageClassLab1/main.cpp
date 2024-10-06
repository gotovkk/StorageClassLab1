﻿#include <iostream>
#include <vector>
#include <variant>
#include "Storage.h"
#include "Product.h"
#include "SellerManager.h"


int main() {
    system("chcp 1251");
    std::cout << "Нажмите Enter, чтобы начать...";
    std::cin.get();

    sqlite3* db;
    SellerManager sellerManager;
    std::string password;
    std::string login;

    int rc;
    int choice;
    int seller_id;
    std::string firstProductName;
    std::string secondProductName;
    Product const* first = nullptr;
    Product const* second = nullptr;

    Storage storage;
    const char* dbPath = "C:\\Users\\anima\\source\\repos\\StorageClassLab1\\mydb.db";

    rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка открытия базы данных: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    createTable(db);


    while (true) {
        std::cout << "\nВыберите действие:\n"
            << "1. Вход существующего продавца\n"
            << "2. Регистрация нового продавца\n"
            << "0. Выход\n"
            << "Ваш выбор: ";

        std::cin >> choice;

        if (choice == 0) {
            sqlite3_close(db);
            return 0; 
        }

        switch (choice) {
        case 1:
            std::cout << "Введите логин:\n";
            std::cin >> login;
            std::cout << "Введите пароль:\n";
            std::cin >> password;

            if (sellerManager.login(db, login, password)) {
                std::cout << "Вход выполнен успешно!\n";
                if (!sellerManager.getSellerIdByUsername(db, login, seller_id)) {
                    std::cerr << "Ошибка получения ID продавца.\n";
                    continue;
                }

                break; 
            }
            else {
                std::cerr << "Ошибка входа. Проверьте логин и пароль.\n";
            }
            break;

        case 2:
            std::cout << "Регистрация нового продавца:\n";
            std::cout << "Введите логин:\n";
            std::cin >> login;
            std::cout << "Введите пароль:\n";
            std::cin >> password;

            if (sellerManager.registerSeller(db, login, password, seller_id)) {
                std::cout << "Регистрация успешна!\n";

            }
            else {
                std::cerr << "Ошибка регистрации продавца." << std::endl;
                sqlite3_close(db);
                return 1; 
            }
            break;

        default:
            std::cout << "Неверный выбор, попробуйте снова.\n";
            break;
        }
        loadProductsFromDb(db, storage);


        while (true) {
            std::cout << "\nВыберите действие:\n"
                << "1. Добавить продукт\n"
                << "2. Удалить продукт\n"
                << "3. Обновить продукт\n"
                << "4. Показать список продуктов\n"
                << "5. Найти товары с низким запасом\n"
                << "6. Сравнить два товара\n"
                << "0. Выход\n"
                << "Ваш выбор: ";

            std::cin >> choice;

            switch (choice) {
            case 1:
                system("cls");
                addToTable(storage, db, seller_id); 
                break;

            case 2:
                system("cls");
                removeProduct(storage);
                break;

            case 3:
                system("cls");
                updateProduct(storage);
                break;

            case 4:
                system("cls");
                showProducts(storage);
                break;

            case 5:
                system("cls");
                findLowStockProducts(storage);
                break;

            case 6:
                system("cls");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Введите имя первого продукта для сравнения: ";
                std::getline(std::cin, firstProductName);
                std::cout << "Введите имя второго продукта для сравнения: ";
                std::getline(std::cin, secondProductName);
                first = findProductByName(storage, firstProductName);
                second = findProductByName(storage, secondProductName);
                comprasionCheck(first, second);
                break;

            case 0:
                sqlite3_close(db);
                return 0;

            default:
                std::cout << "Неверный выбор, попробуйте снова." << std::endl;
            }
        }
    }

    sqlite3_close(db);
    return 0;
}
