#include <windows.h>
#include <stdio.h>
#include <aclapi.h>
#include <locale.h>
#include <string.h>

#define FILENAME "test.txt"

void printError(const char* msg) {
    printf("%s (код ошибки: %lu)\n", msg, GetLastError());
}

//очистка буфера
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//1.создание файла
void createFile() {
    HANDLE hFile = CreateFile(
        FILENAME,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printError("Ошибка создания файла");
        return;
    }

    printf("Файл успешно создан\n");
    CloseHandle(hFile);
}

//2.запись
void writeFileData() {
    HANDLE hFile = CreateFile(
        FILENAME,
        FILE_APPEND_DATA,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printError("Ошибка открытия файла");
        return;
    }

    char data[256];

    printf("Введите текст: ");
    clearInputBuffer();

    if (fgets(data, sizeof(data), stdin) == NULL) {
        printf("Ошибка ввода\n");
        CloseHandle(hFile);
        return;
    }

    DWORD written;
    if (!WriteFile(hFile, data, (DWORD)strlen(data), &written, NULL)) {
        printError("Ошибка записи");
    } else {
        printf("Записано байт: %lu\n", written);
    }

    CloseHandle(hFile);
}

//3.чтение
void readFileData() {
    HANDLE hFile = CreateFile(
        FILENAME,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printError("Ошибка открытия файла");
        return;
    }

    char buffer[256];
    DWORD bytesRead;

    printf("Содержимое файла:\n");

    while (ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

    printf("\n");
    CloseHandle(hFile);
}

//4.атрибут только чтение
void setReadOnlyAttribute() {
    DWORD attrs = GetFileAttributes(FILENAME);

    if (attrs == INVALID_FILE_ATTRIBUTES) {
        printError("Ошибка получения атрибутов");
        return;
    }

    attrs |= FILE_ATTRIBUTE_READONLY;

    if (!SetFileAttributes(FILENAME, attrs))
        printError("Ошибка установки атрибута");
    else
        printf("Атрибут 'только чтение' установлен\n");
}

//5.снять атрибут
void removeReadOnlyAttribute() {
    DWORD attrs = GetFileAttributes(FILENAME);

    if (attrs == INVALID_FILE_ATTRIBUTES) {
        printError("Ошибка получения атрибутов");
        return;
    }

    attrs &= ~FILE_ATTRIBUTE_READONLY;

    if (!SetFileAttributes(FILENAME, attrs))
        printError("Ошибка снятия атрибута");
    else
        printf("Атрибут 'только чтение' снят\n");
}

//6.ACL только чтение с наследованием
void setReadOnlyACL() {
    EXPLICIT_ACCESS ea[2];
    PACL oldDACL = NULL, newDACL = NULL;
    PSECURITY_DESCRIPTOR sd = NULL;

    ZeroMemory(&ea, sizeof(ea));

    //получаем старый ACL
    DWORD res = GetNamedSecurityInfo(
        FILENAME,
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        NULL,
        NULL,
        &oldDACL,
        NULL,
        &sd
    );

    if (res != ERROR_SUCCESS) {
        printf("Ошибка получения текущего ACL: %lu\n", res);
        return;
    }

    //запрет записи
    ea[0].grfAccessPermissions = GENERIC_WRITE;
    ea[0].grfAccessMode = DENY_ACCESS;
    ea[0].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_NAME;
    ea[0].Trustee.ptstrName = "Everyone";

    //разрешение чтения
    ea[1].grfAccessPermissions = GENERIC_READ;
    ea[1].grfAccessMode = GRANT_ACCESS;
    ea[1].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea[1].Trustee.TrusteeForm = TRUSTEE_IS_NAME;
    ea[1].Trustee.ptstrName = "Everyone";

    res = SetEntriesInAcl(2, ea, oldDACL, &newDACL);

    if (res != ERROR_SUCCESS) {
        printf("Ошибка создания ACL: %lu\n", res);
        if (sd) LocalFree(sd);
        return;
    }

    res = SetNamedSecurityInfo(
        FILENAME,
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        NULL,
        NULL,
        newDACL,
        NULL
    );

    if (res == ERROR_SUCCESS)
        printf("ACL: установлен режим 'только чтение'\n");
    else
        printf("Ошибка установки ACL: %lu\n", res);

    if (sd) LocalFree(sd);
    if (newDACL) LocalFree(newDACL);
}

//7.полный доступ с сохранением
void setFullAccessACL() {
    EXPLICIT_ACCESS ea;
    PACL oldDACL = NULL, newDACL = NULL;
    PSECURITY_DESCRIPTOR sd = NULL;

    ZeroMemory(&ea, sizeof(ea));

    DWORD res = GetNamedSecurityInfo(
        FILENAME,
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        NULL,
        NULL,
        &oldDACL,
        NULL,
        &sd
    );

    if (res != ERROR_SUCCESS) {
        printf("Ошибка получения текущего ACL: %lu\n", res);
        return;
    }

    ea.grfAccessPermissions = GENERIC_ALL;
    ea.grfAccessMode = SET_ACCESS;
    ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
    ea.Trustee.ptstrName = "Everyone";

    res = SetEntriesInAcl(1, &ea, oldDACL, &newDACL);

    if (res != ERROR_SUCCESS) {
        printf("Ошибка создания ACL: %lu\n", res);
        if (sd) LocalFree(sd);
        return;
    }

    res = SetNamedSecurityInfo(
        FILENAME,
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        NULL,
        NULL,
        newDACL,
        NULL
    );

    if (res == ERROR_SUCCESS)
        printf("Полный доступ восстановлен\n");
    else
        printf("Ошибка установки ACL: %lu\n", res);

    if (sd) LocalFree(sd);
    if (newDACL) LocalFree(newDACL);
}

//меню
int main() {
    setlocale(LC_ALL, "");
    int choice;

    do {
        printf("\n1 - Создать файл\n");
        printf("2 - Записать\n");
        printf("3 - Прочитать\n");
        printf("4 - Атрибут: только чтение\n");
        printf("5 - Убрать атрибут\n");
        printf("6 - ACL: только чтение\n");
        printf("7 - ACL: полный доступ\n");
        printf("0 - Выход\n");
        printf(">> ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода\n");
            break;
        }

        switch (choice) {
            case 1: createFile(); break;
            case 2: writeFileData(); break;
            case 3: readFileData(); break;
            case 4: setReadOnlyAttribute(); break;
            case 5: removeReadOnlyAttribute(); break;
            case 6: setReadOnlyACL(); break;
            case 7: setFullAccessACL(); break;
        }

    } while (choice != 0);

    return 0;
}