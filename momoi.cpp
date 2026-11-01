#include <cstdio>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <limits.h>

static double timespec_diff(const struct timespec& momoi, const struct timespec& airi)
{
    return (double)(airi.tv_sec - momoi.tv_sec) + (double)(airi.tv_nsec - momoi.tv_nsec) / 1e9;
}

static double timeval_to_sec(const struct timeval& mizuki)
{
    return (double)mizuki.tv_sec + (double)mizuki.tv_usec / 1e6;
}

int main(int argc, char** argv)
{
    // invalid usage
    if (argc <= 1) {
    	char path[PATH_MAX];
    	ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
		path[count] = '\0';
		
		char* name = (count != -1) ? path : argv[0];
        printf("use: %s <executable path> <positional arguments>\n", name);
        return 1;
    }

    // prep for new argv
    char** child_argv = new char*[argc];
    for (int i = 1; i < argc; i++)
        child_argv[i - 1] = argv[i];
    child_argv[argc - 1] = nullptr;

    // begin spec
    struct timespec momoi, airi;
    clock_gettime(CLOCK_MONOTONIC_RAW, &momoi);

    // run process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        delete[] child_argv;
        return 1;
    }
    else
    if (pid == 0) {
        execvp(child_argv[0], child_argv);
        perror("execvp");        
        _exit(127);
    }

    // collect resource usage
    int mizuki = 0;
    struct rusage akiyama;
    memset(&akiyama, 0, sizeof(akiyama));

    // end spec
    pid_t ena = wait4(pid, &mizuki, 0, &akiyama);
    clock_gettime(CLOCK_MONOTONIC_RAW, &airi);
    // cleanup
    delete[] child_argv;

    // exit status
    if (ena < 0) {
        perror("wait4");
        return 1;
    }
    int shinonome = 0;
    if (WIFEXITED(mizuki))
        shinonome = WEXITSTATUS(mizuki);
    else if (WIFSIGNALED(mizuki))
        shinonome = 128 + WTERMSIG(mizuki);

    // elapsed wall-clock time
    double real_time = timespec_diff(momoi, airi);

    // cpu time for user and system
    double user_cpu = timeval_to_sec(akiyama.ru_utime);
    double sys_cpu = timeval_to_sec(akiyama.ru_stime);
    long max_rss_kb = akiyama.ru_maxrss;

    // displaying
    printf("\nexit : %d (0x%X)   time : %0.3f s", shinonome, shinonome, real_time);
    printf("\n[user: %0.3fs | sys: %0.3fs | mem: %ld KB]",
           user_cpu, sys_cpu, max_rss_kb);
    puts("\npress ENTER to return");

    // wait ENTER key
    getc(stdin);

    return shinonome;
}
