#include "headers.h"
volatile sig_atomic_t interrupt_flag = 0;
pid_t current_foreground_pid = -1;
int err = 0;
int main()
{
    signal(SIGTSTP, sigtstp_handler);
    // Keep accepting commands


    const char *filename = "pastevents.txt";

    char direc_cu[1024];
    if (getcwd(direc_cu, sizeof(direc_cu)) == NULL) {
        perror("Error getting current directory");
        return 1;
    }

    // Construct full filepath
    
    snprintf(filepath_past, sizeof(filepath_past), "%s/%s", direc_cu, filename);
    FILE *file = fopen(filepath_past, "r");
    const char *history[MAX_HISTORY_SIZE];
    int history_count = 0;
    load_history(history, &history_count);
    signal(SIGINT, sigint_handler);
    while (1)
    {
        if (interrupt_flag)
        {
            if (getpid() != getpgid(getpid()))
            {
                kill(getpgid(getpid()), SIGINT);
            }
        }
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt();
        char input[4096];
        if (fgets(input, 4096, stdin) == NULL)
        {
            printf("\nStopped Jobs.\n");
            cleanup();
        }
        strtok(input, "\n");
        char initial_command[1024];
        strcpy(initial_command, input);
        check_completed_background_processes();
        int storechecker = 0;
        char *sptoken;
        char delimiter[] = ";";
        sptoken = strtok(input, delimiter);
        char *commands[100];
        int cindex = 0;
        while (sptoken != NULL)
        {
            commands[cindex++] = sptoken;
            sptoken = strtok(NULL, delimiter);
        }
        commands[cindex] = NULL;
        char delimit[] = "&";
        char *spto;
        char *seccom[100];
        int cin2 = 0;
        int bg[100];
        for (int i = 0; i < cindex; i++)
        {
            int n = strlen(commands[i]), ab = 0;
            if (commands[i][n - 1] == '&')
            {
                ab = 1;
                bg[cin2 - 1] = 1;
            }
            if (strchr(commands[i], '&') == NULL)
            {
                bg[cin2] = 0;
                seccom[cin2++] = commands[i];
            }
            else
            {
                spto = strtok(commands[i], delimit);
                int checker = 0;
                while (spto != NULL)
                {
                    bg[cin2] = 1;
                    seccom[cin2++] = spto;
                    spto = strtok(NULL, delimit);
                    checker++;
                }
                if (checker > 1 && ab != 1)
                    bg[cin2 - 1] = 0;
            }
        }
        for (int i = 0; i < cin2; i++)
        {
            err = 0;
            strtok(seccom[i], "\n");
            if (strchr(seccom[i], '|'))
            {
                size_t len = strlen(seccom[i]);
                if (len > 0 && seccom[i][len - 1] == '\n')
                {
                    seccom[i][len - 1] = '\0';
                }

                if (strcmp(seccom[i], "exit") == 0)
                {
                    exit(0);
                }
                char *cands[10];
                int num_commands = 0;
                char *token = strtok(seccom[i], "|");
                while (token != NULL && num_commands < 10)
                {
                    cands[num_commands++] = token;
                    token = strtok(NULL, "|");
                }

                int input_fd = STDIN_FILENO;
                int fd[2];

                for (int i = 0; i < num_commands; i++)
                {
                    char *input_redirect = strstr(cands[i], "<");
                    char *output_redirect = strstr(cands[i], ">");
                    char *append_redirect = strstr(cands[i], ">>");

                    if (i < num_commands - 1)
                    {
                        if (pipe(fd) == -1)
                        {
                            perror("pipe");
                            exit(EXIT_FAILURE);
                        }
                    }

                    pid_t child_pid = fork();

                    if (child_pid == -1)
                    {
                        perror("fork");
                        exit(EXIT_FAILURE);
                    }

                    if (child_pid == 0)
                    {
                        // Child process
                        if (i < num_commands - 1)
                        {
                            dup2(fd[1], STDOUT_FILENO);
                            close(fd[1]);
                        }

                        if (input_fd != STDIN_FILENO)
                        {
                            dup2(input_fd, STDIN_FILENO);
                        }

                        if (input_redirect)
                        {
                            *input_redirect = '\0';
                            char *filename = strtok(input_redirect + 1, " ");
                            FILE *file = fopen(filename, "r");

                            if (file)
                            {
                                dup2(fileno(file), STDIN_FILENO);
                                fclose(file);
                            }
                            else
                            {
                                perror("Failed to open file");
                                exit(EXIT_FAILURE);
                            }
                        }

                        if (output_redirect)
                        {
                            if (append_redirect)
                            {
                                *append_redirect = '\0';
                                char *filename = strtok(append_redirect + 2, " ");

                                FILE *file = fopen(filename, "a");
                                if (file)
                                {
                                    dup2(fileno(file), STDOUT_FILENO);
                                    fclose(file);
                                }
                                else
                                {
                                    perror("Failed to open file");
                                    exit(EXIT_FAILURE);
                                }
                            }
                            else
                            {
                                *output_redirect = '\0';
                                char *filename = strtok(output_redirect + 1, " ");

                                FILE *file = fopen(filename, "w");
                                if (file)
                                {
                                    dup2(fileno(file), STDOUT_FILENO);
                                    fclose(file);
                                }
                                else
                                {
                                    perror("Failed to open file");
                                    exit(EXIT_FAILURE);
                                }
                            }
                        }
                        removeLeadingSpaces(cands[i]);
                        if (strncmp(cands[i], "pastevents execute", 18) == 0)
                        {
                            int num;
                            sscanf(cands[i] + 19, "%d", &num);
                            char focand[1024];
                            strcpy(focand, cands[i]);
                            copy_command(cands[i], history_count - num + 1, history, &history_count);
                            strtok(cands[i], "\n");
                            storechecker = 1;
                            char buffer[4096];
                            char *insert_point = &buffer[0];
                            const char *tmp = initial_command;
                            size_t pattern_length = strlen(focand);
                            while (1)
                            {
                                const char *p = strstr(tmp, focand);
                                if (p == NULL)
                                {
                                    strcpy(insert_point, tmp);
                                    break;
                                }
                                strncpy(insert_point, tmp, p - tmp);
                                insert_point += p - tmp;
                                strcpy(insert_point, cands[i]);
                                insert_point += strlen(cands[i]);
                                tmp = p + pattern_length;
                            }
                            strcpy(initial_command, buffer);
                        }
                        if (strncmp(cands[i], "fg", 2) == 0)
                        {
                            int pid;
                            sscanf(cands[i] + 3, "%d", &pid);
                            char *res = fg(pid);
                            if (res != NULL)
                            {
                                strcpy(cands[i], res);
                            }
                            else
                            {
                                printf("No pid\n");
                                continue;
                            }
                        }
                        if (strncmp(cands[i], "bg", 2) == 0)
                        {
                            int pid;
                            sscanf(cands[i] + 3, "%d", &pid);
                            char *rest = bgp(pid);
                            if (rest != NULL)
                            {
                                strcpy(cands[i], rest);
                                bg[i] = 1;
                            }
                            else
                            {
                                printf("No pid\n");
                                continue;
                            }
                        }
                        if (strncmp(cands[i], "iMan", 4) == 0)
                        {
                            char *arguments = cands[i] + 4;
                            char *toki = strtok(arguments, " ");
                            char *argst[3];
                            int num_to = 0;
                            while (toki != NULL)
                            {
                                argst[num_to++] = toki;
                                toki = strtok(NULL, " ");
                            }
                            if (num_to != 1)
                            {
                                printf("Usage:iMan <command_name>\n");
                            }
                            else
                            {
                                iManspec(argst[0]);
                            }
                        }
                        else if (strcmp(cands[i], "pastevents") == 0)
                        {
                            storechecker = 2;
                            print_history(history, history_count);
                            exit(EXIT_SUCCESS);
                        }
                        else if (strcmp(cands[i], "pastevents purge") == 0)
                        {
                            storechecker = 2;
                            clear_history(history, &history_count);
                        }
                        else if (strncmp(cands[i], "warp", 4) == 0)
                        {
                            char *arguments = cands[i] + 4;
                            char *token = strtok(arguments, " ");
                            if (token == NULL)
                                warp("~");
                            else
                            {
                                while (token != NULL)
                                {
                                    warp(token);
                                    token = strtok(NULL, " ");
                                }
                            }
                            exit(EXIT_SUCCESS);
                        }
                        else if (strncmp(cands[i], "peek", 4) == 0)
                        {
                            spec4(cands[i]);
                            exit(EXIT_SUCCESS);
                        }
                        else if (strncmp(cands[i], "proclore", 8) == 0)
                        {
                            spec7(cands[i]);
                            exit(EXIT_SUCCESS);
                        }
                        else if (strcmp(cands[i], "exit") == 0)
                        {
                            exit(0);
                        }
                        else if (strncmp(cands[i], "ping", 4) == 0)
                        {
                            specping(cands[i]);
                        }
                        else if (strncmp(cands[i], "seek", 4) == 0)
                        {
                            spec8(cands[i]);
                            strcpy(prprinter, "");
                            exit(EXIT_SUCCESS);
                        }
                        else if (strncmp(cands[i], "activities", 10) == 0)
                        {
                            //  add_to_history(cands[i], history, &history_count);
                            list_background_processes();
                            exit(EXIT_SUCCESS);
                        }
                        else if (strncmp(cands[i], "neonate -n", 10) == 0)
                        {
                            handle(cands[i]);
                        }
                        else
                        {
                            char *args[64];
                            int k = 0;
                            char *arg = strtok(cands[i], " \n");
                            while (arg != NULL)
                            {
                                args[k++] = arg;
                                arg = strtok(NULL, " \n");
                            }
                            args[k] = NULL;
                            execvp(args[0], args);
                            perror("execvp");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        if (i < num_commands - 1)
                        {
                            close(fd[1]);
                        }
                        wait(NULL);

                        if (i < num_commands - 1)
                        {
                            input_fd = fd[0];
                        }
                    }
                }
                if (input_fd != STDIN_FILENO)
                {
                    close(input_fd);
                }
            }
            else
            {

                char *input_redirect = strstr(seccom[i], "<");
                char *output_redirect = strstr(seccom[i], ">");
                char *append_redirect = strstr(seccom[i], ">>");
                removeLeadingSpaces(seccom[i]);
                if (strncmp(seccom[i], "pastevents execute", 18) == 0)
                {
                    int num;
                    char sta[1024];
                    strcpy(sta, seccom[i]);
                    sscanf(seccom[i] + 19, "%d", &num);
                    copy_command(seccom[i], history_count - num + 1, history, &history_count);
                    strtok(seccom[i], "\n");
                    char buffer[4096];
                    char *insert_point = &buffer[0];
                    const char *tmp = initial_command;
                    size_t pattern_length = strlen(sta);
                    while (1)
                    {
                        const char *p = strstr(tmp, sta);
                        if (p == NULL)
                        {
                            strcpy(insert_point, tmp);
                            break;
                        }
                        strncpy(insert_point, tmp, p - tmp);
                        insert_point += p - tmp;
                        strcpy(insert_point, seccom[i]);
                        insert_point += strlen(seccom[i]);
                        tmp = p + pattern_length;
                    }
                    strcpy(initial_command, buffer);
                }
                if (strncmp(seccom[i], "fg", 2) == 0)
                {
                    int pid;
                    sscanf(seccom[i] + 3, "%d", &pid);
                    char *res = fg(pid);
                    if (res != NULL)
                    {
                        strcpy(seccom[i], res);
                    }
                    else
                    {
                        printf("No pid\n");
                        continue;
                    }
                }
                if (strncmp(seccom[i], "bg", 2) == 0)
                {
                    int pid;
                    sscanf(seccom[i] + 3, "%d", &pid);
                    char *rest = bgp(pid);
                    if (rest != NULL)
                    {
                        strcpy(seccom[i], rest);
                        bg[i] = 1;
                    }
                    else
                    {
                        printf("No pid\n");
                        continue;
                    }
                }
                if (strncmp(seccom[i], "iMan", 4) == 0)
                {
                    char *arguments = seccom[i] + 4;
                    char *toki = strtok(arguments, " ");
                    char *argst[3];
                    int num_to = 0;
                    while (toki != NULL)
                    {
                        argst[num_to++] = toki;
                        toki = strtok(NULL, " ");
                    }
                    if (num_to != 1)
                    {
                        printf("Usage:iMan <command_name>\n");
                    }
                    else
                    {
                        iManspec(argst[0]);
                    }
                }
                else if (strcmp(seccom[i], "pastevents purge") == 0)
                {
                    storechecker = 2;
                    clear_history(history, &history_count);
                }
                else if (strncmp(seccom[i], "pastevents", 10) == 0)
                {
                    // i/o then printed in file
                    storechecker = 2;
                    int original_stdout = dup(STDOUT_FILENO);
                    int original_stdin = dup(STDIN_FILENO);
                    if (input_redirect)
                    {
                        *input_redirect = '\0';
                        char *filename = strtok(input_redirect + 1, " ");
                        FILE *file = fopen(filename, "r");

                        if (file)
                        {
                            // Redirect stdin to the file.
                            dup2(fileno(file), STDIN_FILENO);
                            fclose(file);
                        }
                        else
                        {
                            perror("Failed to open file");
                        }
                    }
                    if (output_redirect)
                    {
                        if (append_redirect)
                        {
                            *append_redirect = '\0';
                            char *filename = strtok(append_redirect + 2, " ");

                            FILE *file = fopen(filename, "a");
                            if (file)
                            {
                                dup2(fileno(file), STDOUT_FILENO);
                                fclose(file);
                            }
                            else
                            {
                                perror("Failed to open file");
                            }
                        }
                        else
                        {
                            *output_redirect = '\0';
                            char *filename = strtok(output_redirect + 1, " ");

                            FILE *file = fopen(filename, "w");
                            if (file)
                            {
                                dup2(fileno(file), STDOUT_FILENO);
                                fclose(file);
                            }
                            else
                            {
                                perror("Failed to open file");
                            }
                        }
                    }
                    print_history(history, history_count);
                    dup2(original_stdin, STDIN_FILENO);
                    dup2(original_stdout, STDOUT_FILENO);
                    close(original_stdout);
                    close(original_stdin);
                }
                else if (strncmp(seccom[i], "warp", 4) == 0)
                {
                    if (input_redirect)
                    {
                        *input_redirect = '\0';
                        char *filename = strtok(input_redirect + 1, " ");
                        if (access(filename, F_OK) == -1)
                        {
                            printf("%s:No such file or directory\n", filename);
                            continue;
                        }
                        // add_to_history(seccom[i], history, &history_count);
                    }
                    int original_stdout = dup(STDOUT_FILENO);
                    if (output_redirect)
                    {
                        if (append_redirect)
                        {
                            *append_redirect = '\0';
                            char *filename = strtok(append_redirect + 2, " ");

                            FILE *file = fopen(filename, "a");
                            if (file)
                            {
                                dup2(fileno(file), STDOUT_FILENO);
                                fclose(file);
                            }
                            else
                            {
                                perror("Failed to open file");
                            }
                        }
                        else
                        {
                            *output_redirect = '\0';
                            char *filename = strtok(output_redirect + 1, " ");

                            FILE *file = fopen(filename, "w");
                            if (file)
                            {
                                dup2(fileno(file), STDOUT_FILENO);
                                fclose(file);
                            }
                            else
                            {
                                perror("Failed to open file");
                            }
                        }
                    }
                    // add_to_history(seccom[i], history, &history_count);
                    char *arguments = seccom[i] + 4;
                    char *token = strtok(arguments, " ");
                    if (token == NULL)
                        warp("~");
                    else
                    {
                        while (token != NULL)
                        {
                            warp(token);
                            token = strtok(NULL, " ");
                        }
                    }
                    dup2(original_stdout, STDOUT_FILENO);
                    close(original_stdout);
                    // i/o redirect changed path should print into the file
                }
                else if (strncmp(seccom[i], "peek", 4) == 0)
                {
                    if (input_redirect)
                    {
                        *input_redirect = '\0';
                        char *filename = strtok(input_redirect + 1, " ");
                        if (access(filename, F_OK) == -1)
                        {
                            printf("%s:No such file or directory\n", filename);
                            continue;
                        }
                        // add_to_history(seccom[i], history, &history_count);
                    }
                    int original_stdout = dup(STDOUT_FILENO);
                    if (output_redirect)
                    {
                        if (append_redirect)
                        {
                            *append_redirect = '\0';
                            char *filename = strtok(append_redirect + 2, " ");

                            FILE *file = fopen(filename, "a");
                            if (file)
                            {
                                dup2(fileno(file), STDOUT_FILENO);
                                fclose(file);
                            }
                            else
                            {
                                perror("Failed to open file");
                            }
                        }
                        else
                        {
                            *output_redirect = '\0';
                            char *filename = strtok(output_redirect + 1, " ");

                            FILE *file = fopen(filename, "w");
                            if (file)
                            {
                                dup2(fileno(file), STDOUT_FILENO);
                                fclose(file);
                            }
                            else
                            {
                                perror("Failed to open file");
                            }
                        }

                        // add_to_history(seccom[i], history, &history_count);
                        //    spec4(seccom[i]);
                    }
                    spec4(seccom[i]);
                    dup2(original_stdout, STDOUT_FILENO);
                    close(original_stdout);
                }
                else if (strncmp(seccom[i], "proclore", 8) == 0)
                {
                    // add_to_history(seccom[i], history, &history_count);
                    spec7(seccom[i]);
                }
                else if (strcmp(seccom[i], "exit") == 0)
                {
                    if (storechecker != 2)
                        add_to_history(input, history, &history_count);
                    exit(0);
                }
                else if (strncmp(seccom[i], "ping", 4) == 0)
                {
                    specping(seccom[i]);
                }
                else if (strncmp(seccom[i], "seek", 4) == 0)
                {

                    if (input_redirect)
                    {
                        *input_redirect = '\0';
                        char *filename = strtok(input_redirect + 1, " ");
                        if (access(filename, F_OK) == -1)
                        {
                            printf("%s:No such file or directory\n", filename);
                            continue;
                        }
                        // add_to_history(seccom[i], history, &history_count);
                    }
                    int original_stdout = dup(STDOUT_FILENO);
                    if (output_redirect)
                    {
                        if (append_redirect)
                        {
                            *append_redirect = '\0';
                            char *filename = strtok(append_redirect + 2, " ");

                            FILE *file = fopen(filename, "a");
                            if (file)
                            {
                                dup2(fileno(file), STDOUT_FILENO);
                                fclose(file);
                            }
                            else
                            {
                                perror("Failed to open file");
                            }
                        }
                        else
                        {
                            *output_redirect = '\0';
                            char *filename = strtok(output_redirect + 1, " ");

                            FILE *file = fopen(filename, "w");
                            if (file)
                            {
                                dup2(fileno(file), STDOUT_FILENO);
                                fclose(file);
                            }
                            else
                            {
                                perror("Failed to open file");
                            }
                        }

                        // add_to_history(seccom[i], history, &history_count);
                    }
                    spec8(seccom[i]);
                    strcpy(prprinter, "");
                    dup2(original_stdout, STDOUT_FILENO);
                    close(original_stdout);
                }
                else if (strncmp(seccom[i], "activities", 10) == 0)
                {
                    actshelp(input_redirect, output_redirect, append_redirect);
                }
                else if (strncmp(seccom[i], "neonate -n", 10) == 0)
                {
                    handle(seccom[i]);
                }
                else
                {
                    char del[] = " \n";
                    char *token = strtok(seccom[i], del);
                    char *store[100];
                    int ind = 0;
                    if (token != NULL)
                    {

                        while (token != NULL)
                        {
                            store[ind++] = token;
                            token = strtok(NULL, del);
                        }
                        char concatenated_command[1024];
                        strcpy(concatenated_command, store[0]);
                        for (int l = 1; l < ind; l++)
                        {
                            strcat(concatenated_command, " ");
                            strcat(concatenated_command, store[l]);
                        }
                        strcpy(seccom[i], concatenated_command);
                        int original_stdout = dup(STDOUT_FILENO);
                        int original_stdin = dup(STDIN_FILENO);
                        if (input_redirect)
                        {
                            *input_redirect = '\0';
                            char *filename = strtok(input_redirect + 1, " ");
                            FILE *file = fopen(filename, "r");

                            if (file)
                            {
                                // Redirect stdin to the file.
                                dup2(fileno(file), STDIN_FILENO);
                                fclose(file);
                            }
                            else
                            {
                                perror("Failed to open file");
                            }
                        }
                        if (output_redirect)
                        {
                            if (append_redirect)
                            {
                                *append_redirect = '\0';
                                char *filename = strtok(append_redirect + 2, " ");

                                FILE *file = fopen(filename, "a");
                                if (file)
                                {
                                    dup2(fileno(file), STDOUT_FILENO);
                                    fclose(file);
                                }
                                else
                                {
                                    perror("Failed to open file");
                                }
                            }
                            else
                            {
                                *output_redirect = '\0';
                                char *filename = strtok(output_redirect + 1, " ");

                                FILE *file = fopen(filename, "w");
                                if (file)
                                {
                                    dup2(fileno(file), STDOUT_FILENO);
                                    fclose(file);
                                }
                                else
                                {
                                    perror("Failed to open file");
                                }
                            }
                        }
                        spec6(seccom[i], bg[i]);
                        dup2(original_stdin, STDIN_FILENO);
                        dup2(original_stdout, STDOUT_FILENO);
                        close(original_stdout);
                        close(original_stdin);
                    }
                }
            }
        }
        if (storechecker != 2 && strcmp(initial_command, "\n") != 0 && err != 1)
        {
            add_to_history(initial_command, history, &history_count);
        }
    }
    for (int j = 0; j < history_count; j++)
    {
        free((void *)history[j]);
    }
    return 0;
}
