#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_STUDENTS 20
#define MAX_NAME_LENGTH 50
#define MAX_EXAMS 5
#define MAX_TESTS 10
#define MAX_DATE_LENGTH 20
#define MAX_FILENAME_LENGTH 100

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int result;
    Date date;
} Exam;

typedef struct {
    int result;
    Date date;
} Test;

typedef struct {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char patronymic[MAX_NAME_LENGTH];
    Date birthDate;
    Exam exams[MAX_EXAMS];
    int numExams;
    Test tests[MAX_TESTS];
    int numTests;
} Student;

typedef struct {
    Student* students[MAX_STUDENTS];
    int numStudents;
} Group;

Group group;
char databaseFileName[MAX_FILENAME_LENGTH];

void addStudent() {
    if (group.numStudents >= MAX_STUDENTS) {
        printf("Ошибка: достигнуто максимальное количество студентов в группе.\n");
        return;
    }

    Student* student = malloc(sizeof(Student));

    printf("Введите фамилию студента: ");
    scanf("%s", student->lastName);

    printf("Введите имя студента: ");
    scanf("%s", student->firstName);

    printf("Введите отчество студента: ");
    scanf("%s", student->patronymic);

    printf("Введите дату рождения студента (день, месяц, год): ");
    scanf("%d %d %d", &student->birthDate.day, &student->birthDate.month, &student->birthDate.year);

    printf("Введите количество зачетов (не более %d): ", MAX_TESTS);
    scanf("%d", &student->numTests);

    for (int i = 0; i < student->numTests; i++) {
        printf("Введите результат зачета %d (1 - зачет, 0 - незачет): ", i + 1);
        scanf("%d", &student->tests[i].result);

        printf("Введите дату сдачи зачета %d (день, месяц, год): ", i + 1);
        scanf("%d %d %d", &student->tests[i].date.day, &student->tests[i].date.month, &student->tests[i].date.year);
    }

    printf("Введите количество экзаменов (не более %d): ", MAX_EXAMS);
    scanf("%d", &student->numExams);

    for (int i = 0; i < student->numExams; i++) {
        printf("Введите оценку за экзамен %d (от 2 до 5): ", i + 1);
        scanf("%d", &student->exams[i].result);

        printf("Введите дату сдачи экзамена %d (день, месяц, год): ", i + 1);
        scanf("%d %d %d", &student->exams[i].date.day, &student->exams[i].date.month, &student->exams[i].date.year);
    }

    group.students[group.numStudents++] = student;

    printf("Студент успешно добавлен.\n");
}

void printStudentInfo(Student* student) {
    printf("Фамилия: %s\n", student->lastName);
    printf("Имя: %s\n", student->firstName);
    printf("Отчество: %s\n", student->patronymic);
    printf("Дата рождения: %d.%d.%d\n", student->birthDate.day, student->birthDate.month, student->birthDate.year);

    printf("Сданные зачеты:\n");
    for (int i = 0; i < student->numTests; i++) {
        printf("Зачет %d: %s\n", i + 1, student->tests[i].result ? "Зачет" : "Незачет");
        printf("Дата сдачи: %d.%d.%d\n", student->tests[i].date.day, student->tests[i].date.month, student->tests[i].date.year);
    }

    printf("Экзамены:\n");
    for (int i = 0; i < student->numExams; i++) {
        printf("Экзамен %d: %d\n", i + 1, student->exams[i].result);
        printf("Дата сдачи: %d.%d.%d\n", student->exams[i].date.day, student->exams[i].date.month, student->exams[i].date.year);
    }
}

void searchStudentByName() {
    char lastName[MAX_NAME_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char patronymic[MAX_NAME_LENGTH];

    printf("Введите фамилию студента: ");
    scanf("%s", lastName);

    printf("Введите имя студента: ");
    scanf("%s", firstName);

    printf("Введите отчество студента: ");
    scanf("%s", patronymic);

    int found = 0;

    for (int i = 0; i < group.numStudents; i++) {
        Student* student = group.students[i];

        if (strcmp(student->lastName, lastName) == 0 && strcmp(student->firstName, firstName) == 0 &&
            strcmp(student->patronymic, patronymic) == 0) {
            printf("Информация о студенте:\n");
            printStudentInfo(student);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Студент не найден.\n");
    }
}

void searchStudentsBySession() {
    printf("Студенты, сдавшие сессию:\n");

    int found = 0;

    for (int i = 0; i < group.numStudents; i++) {
        Student* student = group.students[i];

        int examsPassed = 1;
        int testsPassed = 1;

        for (int j = 0; j < student->numExams; j++) {
            if (student->exams[j].result < 2) {
                examsPassed = 0;
                break;
            }
        }

        for (int j = 0; j < student->numTests; j++) {
            if (!student->tests[j].result) {
                testsPassed = 0;
                break;
            }
        }

        if (examsPassed && testsPassed) {
            printf("%s %s %s\n", student->lastName, student->firstName, student->patronymic);
            found = 1;
        }
    }

    if (!found) {
        printf("Нет студентов, сдавших сессию.\n");
    }
}

void printDatabase() {
    printf("Информация о всех студентах:\n");

    for (int i = 0; i < group.numStudents; i++) {
        printf("Студент %d:\n", i + 1);
        printStudentInfo(group.students[i]);
        printf("\n");
    }
}

void saveDatabase() {
    FILE* file = fopen(databaseFileName, "wb");

    if (file == NULL) {
        printf("Ошибка при сохранении базы данных.\n");
        return;
    }

    fwrite(&group.numStudents, sizeof(int), 1, file);

    for (int i = 0; i < group.numStudents; i++) {
        fwrite(group.students[i], sizeof(Student), 1, file);
    }

    fclose(file);

    printf("База данных успешно сохранена.\n");
}

void loadDatabase() {
    FILE* file = fopen(databaseFileName, "rb");

    if (file == NULL) {
        printf("Ошибка при загрузке базы данных.\n");
        return;
    }

    fread(&group.numStudents, sizeof(int), 1, file);

    for (int i = 0; i < group.numStudents; i++) {
        Student* student = malloc(sizeof(Student));
        fread(student, sizeof(Student), 1, file);
        group.students[i] = student;
    }

    fclose(file);

    printf("База данных успешно загружена.\n");
}

void deleteStudent() {
    if (group.numStudents == 0) {
        printf("Группа студентов пуста.\n");
        return;
    }

    char lastName[MAX_NAME_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char patronymic[MAX_NAME_LENGTH];

    printf("Введите фамилию студента: ");
    scanf("%s", lastName);

    printf("Введите имя студента: ");
    scanf("%s", firstName);

    printf("Введите отчество студента: ");
    scanf("%s", patronymic);

    int found = 0;

    for (int i = 0; i < group.numStudents; i++) {
        Student* student = group.students[i];

        if (strcmp(student->lastName, lastName) == 0 && strcmp(student->firstName, firstName) == 0 &&
            strcmp(student->patronymic, patronymic) == 0) {
            free(student);

            for (int j = i; j < group.numStudents - 1; j++) {
                group.students[j] = group.students[j + 1];
            }

            group.numStudents--;

            printf("Студент успешно удален.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Студент не найден.\n");
    }
}

void printMenu() {
    printf("\n----- Меню -----\n");
    printf("1. Добавить студента\n");
    printf("2. Найти студента по фамилии, имени и отчеству\n");
    printf("3. Найти студентов, сдавших сессию\n");
    printf("4. Вывести всю информацию из базы данных\n");
    printf("5. Удалить студента\n");
    printf("6. Сохранить базу данных\n");
    printf("7. Загрузить базу данных\n");
    printf("8. Выход\n");
    printf("----------------\n");
    printf("Введите номер операции: ");
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    group.numStudents = 0;

    printf("Введите имя файла с базой данных: ");
    scanf("%s", databaseFileName);

    int choice = 0;

    while (choice != 8) {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                searchStudentByName();
                break;
            case 3:
                searchStudentsBySession();
                break;
            case 4:
                printDatabase();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                saveDatabase();
                break;
            case 7:
                loadDatabase();
                break;
            case 8:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Ошибка: неверный номер операции.\n");
        }
    }

    return 0;
}