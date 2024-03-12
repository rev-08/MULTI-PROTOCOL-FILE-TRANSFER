#include "../../include/comman.h"
#include "colors_effects.h"
#include<sys/stat.h>
#include<unistd.h>
// Function to display menu
void display_menu() {
    printf(YELLOW);
    printf("Samba Configuration Menu\n");
    printf("1\\ Configure an Existing Folder\n");
    printf("2/ Create and Configure a New Folder\n");
    printf("3\\ Restart Samba Service\n");
    printf("4/ Exit\n");
    printf(RESET);
}

// Function to configure an existing folder
void configure_existing_folder() {
    char folder_name[100];
    printf("please provide absolute path");
    printf("Enter the folder name to configure: ");
    fgets(folder_name, sizeof(folder_name), stdin);
    folder_name[strcspn(folder_name, "\n")] = 0; // remove trailing newline
    
    if (strlen(folder_name) == 0) {
        printf("Folder name cannot be empty.\n");
        return;
    }
    
    // Check if folder exists
    if (system(NULL)) {
        if (system("test -d \"$folder_name\"") != 0) {
            printf("Folder '%s' does not exist.\n", folder_name);
            return;
        }
    } else {
        printf("System command not available.\n");
        return;
    }

    if (system(NULL)) 
    {
        if (system("grep -q '^ByteRevolution:' /etc/passwd") != 0) 
        {
            if (system("sudo useradd ByteRevolution -m -s /sbin/nologin") != 0) 
            {
                printf("Failed to create user 'ByteRevolution'.\n");
                return;
            }
            if (system("(echo -e 'revolve\nrevolve' | sudo smbpasswd -a ByteRevolution) && sudo smbpasswd -e ByteRevolution") != 0)
            {
            printf("Failed to set password for 'ByteRevolution'.\n");
            return;
            }
            printf("Password set for user 'ByteRevolution'.\n");
            printf("User 'ByteRevolution' created.\n");
        } 
        else    
        {
            printf("User 'ByteRevolution' already exists.\n");
        }
    } else 
    {
        printf("System command not available.\n");
        return;
    }
    
    // Add configuration to smb.conf
    FILE *smb_conf = fopen("/etc/samba/smb.conf", "a");
    if (smb_conf == NULL) {
        printf("Failed to open smb.conf for writing.\n");       
        return;
    }
    
    fprintf(smb_conf, "[%s]\n", folder_name);
    fprintf(smb_conf, "   path = %s\n", folder_name);
    fprintf(smb_conf, "   browseable = yes\n");
    fprintf(smb_conf, "   writable = yes\n");
    fprintf(smb_conf, "   valid users = ByteRevolution\n");
    
    fclose(smb_conf);
    
    printf("Samba configuration for '%s' added successfully.\n", folder_name);
}

// Function to create and configure a new folder
void create_and_configure_folder() {
    char folder_name[100];
    printf("Enter the folder name to create and configure: ");
    fgets(folder_name, sizeof(folder_name), stdin);
    folder_name[strcspn(folder_name, "\n")] = 0; // remove trailing newline
    
   

    // Remove trailing newline if present
    if (folder_name[strlen(folder_name) - 1] == '\n') {
        folder_name[strlen(folder_name) - 1] = '\0';
    }

    if (system(NULL)) 
    {
        if (system("grep -q '^ByteRevolution:' /etc/passwd") != 0) 
        {
            if (system("sudo useradd ByteRevolution -m -s /sbin/nologin -G ByteRevolution") != 0) 
            {
                printf("Failed to create user 'ByteRevolution'.\n");
                return;
            }
            if (system("(echo -e 'revolve\nrevolve' | sudo smbpasswd -a ByteRevolution) && sudo smbpasswd -e ByteRevolution") != 0) 
            {
            printf("Failed to set password for 'ByteRevolution'.\n");
            return;
        }
            printf("Password set for user 'ByteRevolution'.\n");
            printf("User 'ByteRevolution' created.\n");
        } 
        else 
        {
            printf("User 'ByteRevolution' already exists.\n");
        }    
    } 
    else 
    {
        printf("System command not available.\n");
        return;
    }

    if (strlen(folder_name) == 0) {
        printf("Folder name cannot be empty.\n");
        return;
    }
    
    // Create folder
    if (system(NULL)) {
        
    if ((mkdir(folder_name,0777))!= 0)
    {
        printf("Failed to create folder '%s'.\n", folder_name);
        return;
    }
    } 
    else 
    {
    printf("System command not available.\n");
    return;
}

    
    // // Add configuration to smb.conf
    printf("\nopening the smb.conf file for the writing\n");
    FILE *smb_conf = fopen("/etc/samba/smb.conf","a");
    if (smb_conf == NULL) 
    {
        printf("Failed to open smb.conf for writing.\n");
        return;
    }
    system("bash ./server/src/pwd.sh > path.txt");
    FILE *path_file = fopen("path.txt","r");
    char *path = malloc(100*sizeof(char));
    fscanf(path_file,"%s",path);
    fprintf(smb_conf, "[%s]\n", folder_name);
    fprintf(smb_conf, "   path = %s/%s\n",path,folder_name);//used getenv("PWD") but it was not working
    fprintf(smb_conf, "   browseable = yes\n");
    fprintf(smb_conf, "   writable = yes\n");
    fprintf(smb_conf, "   valid users = ByteRevolution\n");
    free(path);
    fclose(path_file);
    fclose(smb_conf);
    system("exit");
   
    
    printf("Folder '%s' created and configured for sharing.\n", folder_name);
//     system("bash ./server/src/smbconf.sh $folder_name");
}

// Function to restart Samba service
void restart_samba_service() {
    printf("Restarting Samba service...\n");
    system("systemctl restart smbd");
    printf("Samba service restarted.\n");
}

// Main function
int main() {
    printf(RED);
    system("bash ./server/src/package.sh");
    printf(RESET);
    int choice;
    while (1) {
        display_menu();
        PRINT_BOLD("enter your choice :");
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                configure_existing_folder();
                break;
            case 2:
                create_and_configure_folder();
                break;
            case 3:
                restart_samba_service();
                break;
            case 4:
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
                break;
        }
    }
    return 0;
}

