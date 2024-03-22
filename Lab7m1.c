#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void readUsers(const char* filename);
void removeUserFromFile(const char* filename, const char* userIdToRemove);
void addUserToFile(const char* filename);
void generateSubFiles(const char* filename);
void displayMenu();

int main() {
    const char* filename = "users.txt";
    int choice;

    while (1) {
        displayMenu();
        printf("Select an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                readUsers(filename);
                break;
            case 2: {
                char userId[50];
                printf("Enter UserID to remove: ");
                scanf("%s", userId);
                removeUserFromFile(filename, userId);
                break;
            }
            case 3:
                addUserToFile(filename);
                break;
            case 4:
                generateSubFiles(filename);
                break;
            case 5:
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid option, please try again.\n");
        }
    }

    return 0;
}

void displayMenu() {
    printf("\nUser File Management System\n");
    printf("1. Read Users\n");
    printf("2. Remove User\n");
    printf("3. Add User\n");
    printf("4. Generate Sub-Files\n");
    printf("5. Exit\n");
}

void readUsers(const char* filename) {
    // Implement function to read and display users from the file
    char str[1000];
    FILE *file;
    file = fopen("users.txt","r");
    if (file == NULL) {
        perror("Error opening file");
    }
    printf("Contents of users.txt: \n");
    while (fgets(str, 1000, file) != NULL) {
        printf("%s", str);
    }

    fclose(file);
    return;
}

void removeUserFromFile(const char* filename, const char* userIdToRemove) {
    // Implement function to remove a user from the file based on UserID
    FILE* originalFile = fopen(filename, "r");
    if (originalFile == NULL) {
        perror("Error opening file");
        return;
    }

    // 创建临时文件
    const char* tempFilename = "tempfile.tmp";
    FILE* tempFile = fopen(tempFilename, "w");
    if (tempFile == NULL) {
        perror("Error creating temp file");
        fclose(originalFile);
        return;
    }

    char buffer[1024];
    while (fgets(buffer, 1024, originalFile) != NULL) {
        // 检查当前行是否包含要删除的用户ID
        if (strstr(buffer, userIdToRemove) == NULL) {
            // 如果不包含，写入到临时文件
            fputs(buffer, tempFile);
        }
    }

    // 关闭文件
    fclose(originalFile);
    fclose(tempFile);

    // 删除原始文件，并将临时文件重命名为原始文件名
    remove(filename);
    rename(tempFilename, filename);
}

void addUserToFile(const char* filename) {
    // Implement function to add a new user to the file
    // 打开文件以追加模式
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // 获取用户信息
    char userId[50];
    char userName[100];
    char userInfo[256]; // 增加一个用于存储用户信息的变量

    printf("Enter UserID: ");
    scanf("%49s", userId); // 使用%49s限制输入长度，避免溢出
    getchar(); // 捕获并丢弃换行符，以免干扰后续的输入

    printf("Enter UserName: ");
    scanf("%99[^\n]", userName); // 使用%99[^\n]以允许包含空格的输入直到换行符
    getchar(); // 捕获并丢弃换行符

    printf("Enter User Information: ");
    scanf("%255[^\n]", userInfo); // 同样允许输入直到换行符
    getchar(); // 捕获并丢弃换行符

    // 按照指定格式写入文件
    fprintf(file, "\nUserID: %s, Name: %s, Information: %s.\n", userId, userName, userInfo);

    // 关闭文件
    fclose(file);

    printf("User added successfully.\n");
}

void generateSubFiles(const char* filename) {
    // Implement function to generate a sub-file for each user in the main file
    FILE* mainFile = fopen(filename, "r");
    if (mainFile == NULL) {
        perror("Error opening main file");
        return;
    }

    char line[256]; // 假设每行不超过255字符
    while (fgets(line, sizeof(line), mainFile) != NULL) {
        // 解析用户信息
        int userID;
        char name[100], info[150];
        sscanf(line, "UserID: %d, Name: %[^,], Information: %[^\n]", &userID, name, info);

        // 为每个用户创建一个子文件
        char subFilename[20];
        sprintf(subFilename, "%d.txt", userID); // 使用用户ID作为文件名

        FILE* subFile = fopen(subFilename, "w");
        if (subFile == NULL) {
            perror("Error opening sub file");
            continue; // 如果无法创建文件，继续处理下一个用户
        }

        // 写入用户信息到子文件
        fprintf(subFile, "UserID: %d\nName: %s\nInformation: %s\n", userID, name, info);

        // 关闭子文件
        fclose(subFile);
    }

    // 关闭主文件
    fclose(mainFile);
}