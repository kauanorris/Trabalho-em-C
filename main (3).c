#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct
{
  char produtoComprado[20], qntd;
  float precoUnd, total;
} produto;

typedef struct
{
  char nome[50], email[50], cpf[12];
  int id, estadoConta, idCompra;
  float totalGasto;
  produto *p;
} cliente;

FILE *arq;
cliente *c;

int verificaOpcaoValida(int o, int min, int max)
{
  // Verifica se a opção digitada pelo usuário é válida.
  // Se for válida, retorna 1, caso contrário, retorna 0.
  // Ex.: Se as opções para o usuário digitar forem 1, 2 e 3,
  //      'min' será 1, 'max' será 3 e 'o' será o numero digitado.
  //      Se o numero digitado não estiver no intervalo entre 'min' e 'max',
  //      não será válido.

  if (o < min || o > max)
  {
    printf("ERRO: Opção inválida. Digite novamente.\n\n");
    return 0;
  }
  else
  {
    return 1;
  }
}

int buscarCpf(int contador, char cpf[12])
{
  // Busca o cpf digitado pelo usuário em todos os registros.
  // Se encontrar, retorna o índice do registro encontrado.
  // Se não encontrar, retorna -1.

  int idBusca = -1, i;

  for (i = 0; i < contador; i++)
  {
    if (!(strcmp(cpf, c[i].cpf)))
    {
      idBusca = c[i].id;
      break;
    }
  }

  return idBusca;
}

int verificaCpf(char cpf[12], int peso)
{
  // Verifica se o cpf digitado pelo usuário é válido.
  // Retorna 1 se for válido, ou 0 se não for.

  int i, soma = 0, limite = peso - 1, digito = (cpf[limite] - 48), retorno = 1;

  if (strlen(cpf) != 11)
  {
    retorno = 0;
  }
  // Verifica se o cpf digitado tem 11 caracteres.

  else
  {
    for (i = 0; i < 10; i++)
    {
      if (cpf[i] - 48 != cpf[i + 1] - 48)
      {
        break;
      }
      else
      {
        retorno = 0;
      }
    }
    // Verifica se o cpf digitado tem todos os caracteres iguais

    if (retorno)
    {
      for (i = 0; i < limite; i++)
      {
        soma += peso * (cpf[i] - 48);
        peso--;
      }

      soma = soma % 11;
      // Faz as contas para verificar o digito verificador

      if (soma < 2)
      {
        if (digito == 0)
        {
          retorno = 1;
        }
        else
        {
          retorno = 0;
        }
      }
      else
      {
        if (digito == (11 - soma))
        {
          retorno = 1;
        }
        else
        {
          retorno = 0;
        }
      }
      // Verifica se o digito verificador é o válido
    }
  }

  if (!retorno)
  {
    printf("ERRO: CPF inválido. Por favor, digite novamente: \n");
  }

  return retorno;
}

void atualizarBanco(int contador)
{
  // Atualiza o arquivo com os registros dos clientes cadastrados.
  arq = fopen("clientes.txt", "wb");

  fwrite(c, sizeof(cliente), contador, arq);
  // Escreve no arquivo os registros dos clientes cadastrados com base no contador.
  // Ex.: Se o contador for 2, então o arquivo terá 2 registros,
  //      cada um com um tamanho de uma struct 'cliente'.

  fclose(arq);
}

void cadastro(int *contador)
{
  // Cadastra um novo cliente
  int op;

  do
  {
    c = (cliente *)realloc(c, (*contador + 1) * sizeof(cliente));
    // Aumenta dinamicamente o tamanho do vetor struct 'c'
    // em 1 com base no contador.

    c[*contador].p = (produto *)malloc(sizeof(produto));
    // Aloca dinamicamente o vetor struct 'p' em cada registro de 'c'.

    printf("*=============================*\n");
    printf("|    Cadastro de Clientes!    |\n");
    printf("*=============================*\n");

    getchar();
    c[*contador].idCompra = 0;
    c[*contador].id = *contador;
    c[*contador].totalGasto = 0;

    printf("Informe o nome completo: ");
    fgets(c[*contador].nome, 50, stdin);
    c[*contador].nome[strcspn(c[*contador].nome, "\n")] = 0;
    // Remove o \n do final da string digitada.

    fflush(stdin);
    printf("*-----------------------------*\n");

    printf("Informe o e-mail: ");
    scanf("%s", &c[*contador].email);
    printf("*-----------------------------*\n");

    do
    {
      printf("Informe o CPF (somente os dígitos): ");
      scanf("%s", &c[*contador].cpf);

      if (buscarCpf(*contador, c[*contador].cpf) != -1)
      {
        strcpy(c[*contador].cpf, "0");
      }
      // Se o cpf digitado já existir, o cpf é zerado.
    } while (!verificaCpf(c[*contador].cpf, 10) || !verificaCpf(c[*contador].cpf, 11));

    printf("*-----------------------------*\n");
    printf("Cliente cadastrado com sucesso!\n");
    printf("*-----------------------------*\n");
    printf("Deseja continuar a cadastrar?\n");
    printf("0 - Nao\n");
    printf("1 - Sim\n");

    do
    {
      printf("Escolha entre as opções acima: ");
      scanf("%i", &op);
    } while (!(verificaOpcaoValida(op, 0, 1)));

    c[*contador].estadoConta = 1;

    *contador = *contador + 1;

    printf("\e[1;1H\e[2J");
  } while (op);

  atualizarBanco(*contador);
  // Atualiza o banco depois do cadastro
}

void bubbleSort(int contador, int controle)
{
  // Ordena o vetor de clientes 'c' com base no controle.
  // Se controle = 1, ordena por nome e se controle = 2, ordena por ID.

  int i, j;
  cliente temp;

  for (i = 0; i < contador; i++)
  {
    for (j = 0; j < contador - 1; j++)
    {
      if (controle == 1 && (strcmp(c[j].nome, c[j + 1].nome)) > 0)
      {
        // Se o nome do cliente 'j' for maior que o nome do cliente 'j + 1',
        // inverte-se a ordem dos registros.

        temp = c[j];
        c[j] = c[j + 1];
        c[j + 1] = temp;
      }
      else if (controle == 2 && (c[j].id > c[j + 1].id))
      {
        // Se o id do cliente 'j' for maior que o id do cliente 'j + 1',
        // inverte-se a ordem dos registros.

        temp = c[j];
        c[j] = c[j + 1];
        c[j + 1] = temp;
      }
    }
  }

  atualizarBanco(contador);
  // Atualiza o banco depois da ordenação.
}

void listaDeClientes(int contador)
{
  // Lista todos os clientes cadastrados.
  
  cliente lista[contador];
  int o, i, j;

  printf("Voce deseja que esteja ordenado por nome ou por ID?\n");
  printf("1 - Nome\n");
  printf("2 - ID\n");

  //  Verifica qual ordem o cliente deseja para a ordem da lista.
  do
  {
    printf("Escolha entre as opções acima: ");
    scanf("%i", &o);
  } while (!(verificaOpcaoValida(o, 1, 2)));

  bubbleSort(contador, o);

  arq = fopen("clientes.txt", "rb");

  for (i = 0; i < contador; i++)
  {
    // Lê os dados do cliente do arquivo para o vetor 'lista'.
    fread(&lista, sizeof(lista), 1, arq);
    

    // Começa a listagem da conta se a conta estiver ativa.
    if (lista[i].estadoConta)
    {
      printf("*-----------------------------*\n");
      printf("ID: %i\n", lista[i].id);
      printf("Nome: %s\n", lista[i].nome);
      printf("E-mail: %s\n", lista[i].email);
      printf("CPF: %s\n", lista[i].cpf);
      printf("Produtos comprados:\n");

      for (j = 0; j < lista[i].idCompra; j++)
      {
        printf("	%i - Produto: %s\n", (j + 1), lista[i].p[j].produtoComprado);
        printf("	>> Quantidade: %i\n", lista[i].p[j].qntd);
        printf("	>> Total: R$ %.2f\n", lista[i].p[j].total);
      }

      printf("Total gasto: R$ %.2f\n", lista[i].totalGasto);
      printf("*-----------------------------*\n");
    }
  }

  fclose(arq);
}

int buscarCliente(int contador)
{
  // Busca um cliente específico.
  int idBusca;
  char cpf[12];
  cliente busca;

  printf("Digite o CPF do cliente: ");
  scanf("%s", &cpf);

  // Chama a função buscarCpf para ver se o cliente existe.
  idBusca = buscarCpf(contador, cpf);

  if (idBusca == -1)
  {
    printf("ERRO: CPF não encontrado!\n");
  }

  else
  {
    // Abre o arquivo posiciona o ponteiro do arquivo no registro calculado com base em idBusca 
    // E lê os dados desse registro para a variável busca.
    arq = fopen("clientes.txt", "rb");
    fseek(arq, idBusca * sizeof(cliente), SEEK_SET);
    fread(&busca, sizeof(cliente), 1, arq);

    printf("*-----------------------------*\n");
    printf("Cliente encontrado: \n");
    printf("ID: %i\n", busca.id);
    printf("Nome: %s\n", busca.nome);
    printf("E-mail: %s\n", busca.email);
    printf("CPF: %s\n", busca.cpf);
    printf("*-----------------------------*\n");

    fclose(arq);
  }
  // Retorna o ID do cliente encontrado ou -1 se não foi encontrado.
  return idBusca;
}

void desativar(int contador)
{
  // Desativa um cliente especifico.
  int op, idBusca;

  do
  {
    idBusca = buscarCliente(contador);
    if (idBusca > -1)
    {
      printf("Tem certeza que deseja desativar esse cliente? (Você não poderá reativá-lo)\n");
      printf("0 - Nao\n");
      printf("1 - Sim\n");

      do
      {
        printf("Escolha entre as opções acima: ");
        scanf("%i", &op);
      } while (!(verificaOpcaoValida(op, 0, 1)));

      if (op)
      {
        // Coloca o estado da conta para desativado e libera a memoria alocada.   
        c[idBusca].estadoConta = 0;
        
        // Libera a memória alocada para os produtos comprados pelo cliente.
        free(c[idBusca].p);

        // Atualiza o arquivo "clientes.txt" com as alterações.
        atualizarBanco(contador);

        printf("*-----------------------------*\n");
        printf("Cliente desativado com sucesso!\n");
        printf("*-----------------------------*\n");
        printf("Deseja desativar outro cliente?\n");
        printf("0 - Nao\n");
        printf("1 - Sim\n");

        do
        {
          printf("Escolha entre as opções acima: ");
          scanf("%i", &op);
        } while (!(verificaOpcaoValida(op, 0, 1)));
      }

      printf("\e[1;1H\e[2J");
    }
  } while (op);
}

void comprarProduto(int contador)
{
  // Compra um produto específico.
  int op, qntd, idBusca = buscarCliente(contador), idCompra, loop, contadorCompras = 0, i;
  float totalCompra = 0.0, precos[5] = {6.0, 5.0, 3.0, 4.0, 5.0};
  char prods[5][20] = {"cafe", "cha", "refrigerante", "cookie", "sanduiche"};

  if (idBusca != -1)
  {
    do
    {
      printf("*===============================*\n");
      printf("|       Lista de Produtos!      |\n");
      printf("*===============================*\n");
      printf("| 1 - Café.              R$6,00 |\n");
      printf("| 2 - Chá.               R$5,00 |\n");
      printf("| 3 - Refrigerante.      R$3,00 |\n");
      printf("| 4 - Cookie.            R$4,00 |\n");
      printf("| 5 - Sanduíche Natural. R$5,00 |\n");
      printf("*===============================*\n");

      do
      {
        printf("Escolha entre as opções acima: ");
        scanf("%i", &op);
      } while (!(verificaOpcaoValida(op, 0, 5)));

      // Obtém o ID da compra atual do cliente. 
      idCompra = (c[idBusca].idCompra);

      printf("*-----------------------------*\n");
      do
      {
        printf("Digite a quantidade que deseja comprar (máx.: 30): ");
        scanf("%i", &qntd);
      } while (!(verificaOpcaoValida(qntd, 1, 30)));

      // Atualiza os dados da compra atual do cliente.
      c[idBusca].p[idCompra].precoUnd = precos[op - 1];

      c[idBusca].p[idCompra].total = (qntd * (precos[op - 1]));

      printf("*-----------------------------*\n");
      printf("Total parcial: R$ %.2f\n", c[idBusca].p[idCompra].total);

      // Atualiza o total da compra.
      totalCompra += c[idBusca].p[idCompra].total;

      strcpy(c[idBusca].p[idCompra].produtoComprado, prods[op - 1]);

      c[idBusca].p[idCompra].qntd = qntd;

      c[idBusca].totalGasto += c[idBusca].p[idCompra].total;

      // Incrementa o contador de compras e o ID da compra.
      contadorCompras++;

      atualizarBanco(contador);

      printf("*-----------------------------*\n");
      printf("Deseja comprar mais alguma coisa?\n");
      printf("0 - Nao\n");
      printf("1 - Sim\n");

      do
      {
        printf("Escolha entre as opções acima: ");
        scanf("%i", &loop);
      } while (!(verificaOpcaoValida(loop, 0, 1)));

      c[idBusca].idCompra++;

      // Realoca a memória para os produtos comprados.
      c[idBusca].p = (produto *)realloc(c[idBusca].p, (c[idBusca].idCompra + 1) * sizeof(produto *));

      printf("\e[1;1H\e[2J");
    } while (loop);

    printf("Deseja imprimir a nota fiscal?:\n");
    printf("0 - Nao\n");
    printf("1 - Sim\n");

    do
    {
      printf("Escolha entre as opções acima: ");
      scanf("%i", &op);
    } while (!(verificaOpcaoValida(op, 0, 1)));

    printf("\e[1;1H\e[2J");

    if (op)
    {
      printf("*===============================*\n");
      printf("|          Nota fiscal          |\n");
      printf("*===============================*\n");
      for (i = (c[idBusca].idCompra - contadorCompras); i < c[idBusca].idCompra; i++)
      {
        printf("%s: %i und(s). X R$ %.2f = R$ %.2f \n", c[idBusca].p[i].produtoComprado, c[idBusca].p[i].qntd, c[idBusca].p[i].precoUnd, c[idBusca].p[i].total);
      }
      printf("*-----------------------------*\n");
      printf("Total a ser pago: R$ %.2f\n", totalCompra);
      printf("*-----------------------------*\n");
    }
  }
}

int main()
{
  setlocale(LC_ALL, "Portuguese");
  int op, contador = 0;

  // Reserva um espaço da memoria para armazenar da struct
  c = (cliente *)malloc(sizeof(cliente));

  printf("  ____       __      _            _       \n");
  printf(" / ___|__ _ / _| ___| |_ ___ _ __(_) __ _ \n");
  printf("| |   / _` | |_ / _ \\ __/ _ \\ '__| |/ _` |\n");
  printf("| |__| (_| |  _|  __/ ||  __/ |  | | (_| |\n");
  printf(" \\____\\__,_|_|  \\___|\\__\\___|_|  |_|\\__,_|\n");
  printf(" \n");
  printf(" \n");

  printf("*=============================*\n");
  printf("| Bem vindo a nossa cafeteria!|\n");
  do
  {
    printf("*=============================*\n");
    printf("|       Menu de opções        |\n");
    printf("*=============================*\n");
    printf("| 1 - Cadastrar Cliente.      |\n");
    printf("| 2 - Listar Clientes.        |\n");
    printf("| 3 - Desativar Cliente.      |\n");
    printf("| 4 - Procurar Cliente.       |\n");
    printf("| 5 - Comprar Produto.        |\n");
    printf("| 0 - Sair.                   |\n");
    printf("*=============================*\n");

    do
    {
      printf("Escolha entre as opções acima: ");
      scanf("%i", &op);
      printf("\n");
    } while (!(verificaOpcaoValida(op, 0, 5)));

    printf("\e[1;1H\e[2J");

    // Chama a função correspondente à opção escolhida
    switch (op)
    {
    case 1:
      cadastro(&contador);
      break;

    case 2:
      printf("*=============================*\n");
      printf("|      Lista de Clientes!     |\n");
      printf("*=============================*\n");

      listaDeClientes(contador);
      break;

    case 3:
      printf("*===============================*\n");
      printf("|       Desativar Cliente!      |\n");
      printf("*===============================*\n");

      desativar(contador);
      break;

    case 4:
      printf("*===============================*\n");
      printf("|         Buscar Cliente!       |\n");
      printf("*===============================*\n");

      buscarCliente(contador);
      break;

    case 5:
      printf("*===============================*\n");
      printf("|        Comprar Produtos!      |\n");
      printf("*===============================*\n");

      comprarProduto(contador);
      break;

    case 0:
      printf("Obrigado por visitar nossa cafeteria. Volte em breve!\n");
      
      free(c);
      break;
    }
  } while (op);

  return 0;
}
