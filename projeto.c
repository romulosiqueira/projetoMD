#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int mdc(int a, int b) //algoritmo de euclides
{
    if (a % b == 0) return b;
    return mdc(b, a % b);
}

int eh_primo(int n, int antecessor_n) //1: primo| 0: nao eh primo
{
    if (antecessor_n == 1) return 1;
    else return (n % antecessor_n == 0) ? 0 : eh_primo(n, antecessor_n-1);
}

long long int parte_da_tabela(int i, int j, long long int tabelinha_rs[], long long int quocientes[], int quantidade_termos, long long int a_fixo, long long int b_fixo) // 'i' para o array quocientes[] e 'j' para array tabelinha_rs[]
{
    while (i >= 0)
    {
        i--;
        tabelinha_rs[j+1] = (quocientes[i] * tabelinha_rs[j]) + tabelinha_rs[j-1];
        j++;
    }

    if (quantidade_termos % 2 == 0) tabelinha_rs[j-2] = tabelinha_rs[j-2] * (-1);
    else tabelinha_rs[j-1] = tabelinha_rs[j-1] * (-1);

    long long int maiorValor; long long int menorValor;
    if (abs(tabelinha_rs[j-1]) >= abs(tabelinha_rs[j-2]))
    {
        maiorValor = tabelinha_rs[j-1];
        menorValor = tabelinha_rs[j-2];
    }
    else
    {
        maiorValor = tabelinha_rs[j-2];
        menorValor = tabelinha_rs[j-1];
    }

    if (a_fixo < b_fixo)
    {
        if (maiorValor > b_fixo || maiorValor <= 0) //condição do valor de a barra em relação ao cara que tá no mod
        {
            while (maiorValor > b_fixo || maiorValor <= 0) maiorValor += b_fixo;
            return maiorValor;
        }
        else return maiorValor;
    }
    else
    {
        if (menorValor > b_fixo || menorValor <= 0) //condição do valor de a barra em relação ao cara que tá no mod
        {
            while (menorValor > b_fixo || menorValor <= 0) menorValor += b_fixo;
            return menorValor;
        }
        else return menorValor;
    }
}

long long int armazenar_valores(long long int a, long long int b, long long int quocientes[], long long int restos[], int i, long long int a_fixo, long long int b_fixo)
{
    while (restos[i] != 0)
    {
        i++;
        quocientes[i] = b / a;
        restos[i] = b % a;
        b = a;
        a = restos[i];
    }

    long long int tabelinha_rs[1000];
    tabelinha_rs[0] = quocientes[i - 1];
    tabelinha_rs[1] = (quocientes[i - 2] * tabelinha_rs[0]) + 1;
    
    return parte_da_tabela(i - 2, 1, tabelinha_rs, quocientes, i, a_fixo, b_fixo);
}

long long int achandoD(long long int a, long long int b, long long int quocientes[], long long int restos[])
{
    long long int x=a; long long int y=b;
    restos[0] = b % a; quocientes[0] = b / a;
    b = a; a = restos[0];
    
    return armazenar_valores(a, b, quocientes, restos, 0, x, y);
}

long long int gerar_chaves(long long int e, long long int n, long long int z, int operacao)
{
    if (operacao == 1)
    {
        //CHAVE PUBLICA
        FILE *chavePublica;
        chavePublica = fopen("chave_publica.txt", "w");
        fprintf(chavePublica, "%lld\n%lld", e, n);
        fclose(chavePublica);

        printf("\nO arquivo 'chave_publica.txt' foi salvo com sucesso!\n\n");
        return 0;
    }
    else if (operacao == 3)
    {
        //CHAVE PRIVADA
        long long int quocientes[1000]; long long int restos[1000]; //1000 é aleatório.
        long long int d = achandoD(e, z, quocientes, restos); //Que eh o inverso de "e mod z" (e.d congru. 1 mod z)
        return d;
    }
}

long long int pre_gerar_chaves() //para o calculo das variaveis a serem usadas para gerar as chaves(publica e privada)
{
    long long int p, q, e, z, n;
    printf("\nDigite os valores de 'p' e 'q' (sabendo que eles sao primos)\n");
    printf("p = "); scanf("%lld", &p);
    printf("q = "); scanf("%lld", &q);
    if (p < 2 || q < 2) {printf("ERROR 404: not found 'p' and 'q' primos\n"); pre_gerar_chaves();}
    printf("Digite o valor de 'e' (relativamente primo a 'z = (p - 1) . (q - 1)'): "); scanf("%lld", &e);
    printf("\n");
    n = p * q; z = (p - 1) * (q - 1);
    
    if (eh_primo(p, p-1) == 1 && eh_primo(q, q-1) == 1)
    {
        if (mdc(e, z) == 1)
        {
            gerar_chaves(e, n, z, 1);
            return 0;
        }
        else {printf("ERROR 405: mdc(e, z) eh diferente de 1\n"); pre_gerar_chaves();}
    }
    else {printf("ERROR 404: not found 'p' and 'q' primos\n"); pre_gerar_chaves();}
}

long long int enumerado(char texto[]) //converte cada caractere digitado em seu respectivo inteiro de acordo com
{//a tabela do prof (A=2, B=3, ... Z=27 space=28)
    FILE *mensagem_enumerada;
    
    for (int i = 0; i < strlen(texto); i++)
    {
        int aux = texto[i];
        if (aux == 32) //O espaco
        {
            mensagem_enumerada = fopen("numerado.txt", "a");
            aux = 28;
            fprintf(mensagem_enumerada, "%d", aux);
        }
        else if (aux >= 97 && aux <= 122) //Letras minusculas
        {
            mensagem_enumerada = fopen("numerado.txt", "a");
            aux -= 95;
            fprintf(mensagem_enumerada, "%d", aux);
        }
        else if (aux >= 65 && aux <= 90) //Letras maiusculas
        {
            mensagem_enumerada = fopen("numerado.txt", "a");
            aux -= 63;
            fprintf(mensagem_enumerada, "%d", aux);
        }
        fclose(mensagem_enumerada);
    }

    mensagem_enumerada = fopen("numerado.txt", "r"); //para ler o arquivo "numerado.txt"
    long long int textoEnumerado;
    fscanf(mensagem_enumerada, "%lld", &textoEnumerado); //inserir o numero de "numerado.txt" em 'textoEnumerado'
    fclose(mensagem_enumerada);

    return textoEnumerado;
}

void encriptar()
{
    FILE *encriptacao;
    char texto[200];
    printf("Digite o texto a ser encriptado: "); scanf("%[^\n]s", texto);

    long long int num = enumerado(texto);
    int e, n;
    printf("Digite as chaves publicas ('e' e 'n', respectivamente): \n"); scanf("%d%d", &e, &n);

    long long int aux = num;
    for (int i=1; i < e; i++) num *= aux;

    num %= n;
    encriptacao = fopen("mensagem_encriptada.txt", "w");
    fprintf(encriptacao, "%lld", num);
    fclose(encriptacao);
    printf("\nMENSAGEM ENCRIPTADA COM SUCESSO!\n\n");
}

void desencriptar()
{
    FILE *desencriptacao;
    long long int p, q, e, z, n, d;
    printf("\nDigite os valores de 'p' e 'q' (sabendo que eles sao primos)\n");
    printf("p = "); scanf("%lld", &p);
    printf("q = "); scanf("%lld", &q);
    if (p < 2 || q < 2) {printf("ERROR 404: not found 'p' and 'q' primos\n"); desencriptar();}
    printf("\nDigite o valor de 'e' (relativamente primo a 'z = (p - 1) . (q - 1)'): "); scanf("%lld", &e);
    printf("\n");
    n = p * q; z = (p - 1) * (q - 1);

    if (eh_primo(p, p-1) == 1 && eh_primo(q, q-1) == 1)
    {
        if (mdc(e, z) == 1) d = gerar_chaves(e, n, z, 3);
        else {printf("ERROR 405: mdc(e, z) eh diferente de 1\n"); desencriptar();}
    }
    else {printf("ERROR 404: not found 'p' and 'q' primos\n"); desencriptar();}
    printf("Aqui esta o 'd': %lld", d);
    int opcao;
    printf("(1)Digitar a mensagem encriptada\n(2)Ler um arquivo '.txt' com a mensagem\n\nDigite sua opcao: ");
    scanf("%d", &opcao);
    if (opcao == 1)
    {
        long long int case1;
        printf("Digite o texto criptografado: "); scanf("%lld", &case1);
        long long int aux = case1;
        for (int i=1; i < d; i++) case1 *= aux;
        case1 %= n;
        desencriptacao = fopen("mensagem_desencriptada.txt", "w");
        fprintf(desencriptacao, "%lld", case1);
        fclose(desencriptacao);
        printf("\nMENSAGEM DESENCRIPTADA COM SUCESSO!\n\n");
    }
    else if (opcao == 2)
    {
        printf("\nInsira o arquivo 'mensagem_encriptada.txt' no diretorio do arquivo codigo-fonte\n");
        do
        {
            printf("\nDigite 1 para prosseguir: ");
            scanf("%d", &opcao);
            if (opcao != 1) printf("Voce nao digitou a tecla correta :)\n");
        } while (opcao != 1);
        
        desencriptacao = fopen("mensagem_encriptada.txt", "r");
        long long int encriptado;
        fscanf(desencriptacao, "%lld", &encriptado); //inserir o numero de "mensagem_encriptada.txt" em 'encriptado'
        fclose(desencriptacao);

        long long int aux = encriptado;
        for (int i=1; i < d; i++) encriptado *= aux;
        encriptado %= n;

        desencriptacao = fopen("mensagem_desencriptada.txt", "w");
        fprintf(desencriptacao, "%lld", encriptado);
        fclose(desencriptacao);
        printf("\nMENSAGEM DESENCRIPTADA COM SUCESSO!\n\n");
    }
}

void main()
{
    void limpaBuffer(void);
    int opcao;
    printf("------------------------\n");
    printf("--->CRIPTOGRAFIA RSA<---\n");
    printf("------------------------\n");
    printf("1: Gerar chave publica\n2: Encriptar\n3: Desincriptar\n'Qualquer coisa': Sair\nDigite a opcao: ");
    scanf("%d", &opcao);
    if (opcao == 1 || opcao == 2 || opcao == 3)
    {
        limpaBuffer();
        if (opcao == 1) {pre_gerar_chaves(); main();}
        else if (opcao == 2) {encriptar(); main();}
        else {desencriptar(); main();}
    }
    else printf("Flw!\n");
}
void limpaBuffer(void)
{
    char c;
    while ((c = getchar())!= '\n' && c != EOF);
}