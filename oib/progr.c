#include <windows.h>
#include <stdio.h>
#include <aclapi.h>
#include <locale.h>
#include <string.h>
#include <lmcons.h>

#define FILENAME "test.txt"

void printError(const char* msg) {
    printf("%s (error code: %lu)\n", msg, GetLastError());
}

//очистка буфера
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//получение текущего пользователя
void getCurrentUser(char* username) {
    DWORD size = UNLEN + 1;
    GetUserName(username, &size);
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
        printError("Create file failed");
        return;
    }

    printf("File created successfully\n");
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
        printError("Open file failed");
        return;
    }

    char data[256];

    printf("Enter text: ");
    clearInputBuffer();

    if (fgets(data, sizeof(data), stdin) == NULL) {
        printf("Input failed\n");
        CloseHandle(hFile);
        return;
    }

    DWORD written;
    if (!WriteFile(hFile, data, (DWORD)strlen(data), &written, NULL)) {
        printError("Write failed");
    } else {
        printf("Written bytes: %lu\n", written);
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
        printError("Open file failed");
        return;
    }

    char buffer[256];
    DWORD bytesRead;

    printf("File content:\n");

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
        printError("Get attributes failed");
        return;
    }

    attrs |= FILE_ATTRIBUTE_READONLY;

    if (!SetFileAttributes(FILENAME, attrs))
        printError("Set attribute failed");
    else
        printf("Read-only attribute applied\n");
}

//5.снять атрибут
void removeReadOnlyAttribute() {
    DWORD attrs = GetFileAttributes(FILENAME);

    if (attrs == INVALID_FILE_ATTRIBUTES) {
        printError("Get attributes failed");
        return;
    }

    attrs &= ~FILE_ATTRIBUTE_READONLY;

    if (!SetFileAttributes(FILENAME, attrs))
        printError("Remove attribute failed");
    else
        printf("Read-only attribute removed\n");
}

//6.ACL только чтение 
void setReadOnlyACL() {
    char username[UNLEN + 1];
    getCurrentUser(username);

    EXPLICIT_ACCESS ea;
    PACL newDACL = NULL;

    ZeroMemory(&ea, sizeof(ea));

    ea.grfAccessPermissions = GENERIC_READ;
    ea.grfAccessMode = SET_ACCESS;
    ea.grfInheritance = NO_INHERITANCE;

    ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
    ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
    ea.Trustee.ptstrName = username;

    DWORD res = SetEntriesInAcl(1, &ea, NULL, &newDACL);

    if (res != ERROR_SUCCESS) {
        printf("ACL creation failed: %lu\n", res);
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
        printf("Read-only ACL applied for user: %s\n", username);
    else
        printf("ACL set failed: %lu\n", res);

    if (newDACL) LocalFree(newDACL);
}

//7.полный доступ с сохранением
void setFullAccessACL() {
    char username[UNLEN + 1];
    getCurrentUser(username);

    EXPLICIT_ACCESS ea;
    PACL newDACL = NULL;

    ZeroMemory(&ea, sizeof(ea));

    ea.grfAccessPermissions = GENERIC_ALL;
    ea.grfAccessMode = SET_ACCESS;
    ea.grfInheritance = NO_INHERITANCE;

    ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
    ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
    ea.Trustee.ptstrName = username;

    DWORD res = SetEntriesInAcl(1, &ea, NULL, &newDACL);

    if (res != ERROR_SUCCESS) {
        printf("ACL creation failed: %lu\n", res);
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
        printf("Full access restored for user: %s\n", username);
    else
        printf("ACL set failed: %lu\n", res);

    if (newDACL) LocalFree(newDACL);
}

//меню
int main() {
    setlocale(LC_ALL, ".UTF-8");
    int choice;

    do {
        printf("\n1 - Create file\n");
        printf("2 - Write\n");
        printf("3 - Read\n");
        printf("4 - Attribute: read only\n");
        printf("5 - Remove attribute\n");
        printf("6 - ACL: read only\n");
        printf("7 - ACL: full access\n");
        printf("0 - Exit\n");
        printf(">> ");

        if (scanf("%d", &choice) != 1) {
            printf("Input error\n");
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
