#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_USUARIOS 8

pthread_mutex_t mutex_permissao; 

typedef struct {
    int id;
    int tem_permissao; // 1 = permitido, 0 = negado
} Usuario;

// função que cada thread vai executar
void* verificar_permissao(void* arg) {
    Usuario* usuario = (Usuario*) arg;

    printf("Usuario %d solicitou acesso ao recurso.\n", usuario->id);

    // tenta acessar a verificação de permissão
    pthread_mutex_lock(&mutex_permissao);

    if (usuario->tem_permissao)
    {
        printf("Usuario %d tem permissao. Acessando recurso...\n", usuario->id);
        sleep(2);
    } else {
        printf("Usuario %d nao tem permissao. Acesso negado.\n", usuario->id);
    }

    pthread_mutex_unlock(&mutex_permissao);

    pthread_exit(NULL);
    
}

int main() {
    pthread_t threads[NUM_USUARIOS];
    Usuario usuarios[NUM_USUARIOS] = {
        {1, 1}, // usuario 1 tem permissao
        {2, 0}, // usuario 2 NAO tem permissao
        {3, 1}, // usuario 3 tem permissao
        {4, 1}, // usuario 4 tem permissao
        {5, 0}, // usuario 5 NAO tem permissao
        {6, 0}, // usuario 6 NAO tem permissao
        {7, 1}, // usuario 7 tem permissao
        {8, 0}  // usuario 8 NAO tem permissao
    };

    pthread_mutex_init(&mutex_permissao,  NULL); // inicia o mutex

    // criando threads para cada usuario
    for (int i = 0; i < NUM_USUARIOS; i++)
    {
        pthread_create(&threads[i], NULL, verificar_permissao, &usuarios[i]);
    }

    // espera todas as threads terminarem
    for (int i = 0; i < NUM_USUARIOS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex_permissao); // destroi o mutex
    
    printf("Sistema finalizado.\n");
    
    return 0;
}