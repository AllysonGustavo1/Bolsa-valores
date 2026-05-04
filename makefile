CC := gcc
CSTD := -std=c11
WARN := -Wall -Wextra
OPT := -O2

APP := bolsa_cli
TEST_APP := bolsa_tests

CLI_SRC := interface_bolsa_valores.c
DEMO_SRC := demo_bolsa.c
B_CODEGEN_SRC := Bolsa_Valores_Implementation.c
TEST_SRC := test_scenarios.c
HEADERS := bolsa_api.h console_ui.h demo_bolsa.h Bolsa_Valores_Abstract.h Bolsa_Contexto.h

COMMON_FLAGS := $(CSTD) $(WARN) $(OPT) -include bolsa_api.h

.PHONY: all run demo test cli-test asan clean check-files help

all: $(APP)

$(APP): $(CLI_SRC) $(DEMO_SRC) $(B_CODEGEN_SRC) $(HEADERS)
	$(CC) $(COMMON_FLAGS) $(CLI_SRC) $(DEMO_SRC) $(B_CODEGEN_SRC) -o $(APP)

run: $(APP)
	./$(APP)

demo: $(APP)
	printf "s\nestado\ntotal\nestado\nparcial\nestado\ninvalida\ncancelar\nativo2\nestado\nsair\n" | NO_COLOR=1 ./$(APP)

test: $(TEST_APP)
	./$(TEST_APP)

$(TEST_APP): $(TEST_SRC) $(DEMO_SRC) $(B_CODEGEN_SRC) $(HEADERS)
	$(CC) $(COMMON_FLAGS) $(TEST_SRC) $(DEMO_SRC) $(B_CODEGEN_SRC) -o $(TEST_APP)

cli-test: $(APP)
	printf "s\nestado\ntotal\nestado\nparcial\nestado\ninvalida\ncancelar\nativo2\nestado\nsair\n" | NO_COLOR=1 ./$(APP) > run_output_cli.txt
	@echo "Saida salva em run_output_cli.txt"

asan: $(CLI_SRC) $(DEMO_SRC) $(B_CODEGEN_SRC) $(HEADERS)
	$(CC) $(CSTD) $(WARN) -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer \
		-include bolsa_api.h $(CLI_SRC) $(DEMO_SRC) $(B_CODEGEN_SRC) -o $(APP)_asan
	./$(APP)_asan

check-files:
	@test -f $(CLI_SRC) || (echo "Arquivo ausente: $(CLI_SRC)" && exit 1)
	@test -f $(DEMO_SRC) || (echo "Arquivo ausente: $(DEMO_SRC)" && exit 1)
	@test -f $(B_CODEGEN_SRC) || (echo "Arquivo ausente: $(B_CODEGEN_SRC)" && exit 1)
	@test -f bolsa_api.h || (echo "Arquivo ausente: bolsa_api.h" && exit 1)
	@test -f console_ui.h || (echo "Arquivo ausente: console_ui.h" && exit 1)
	@test -f demo_bolsa.h || (echo "Arquivo ausente: demo_bolsa.h" && exit 1)
	@test -f Bolsa_Valores_Abstract.h || (echo "Arquivo ausente: Bolsa_Valores_Abstract.h" && exit 1)
	@test -f Bolsa_Contexto.h || (echo "Arquivo ausente: Bolsa_Contexto.h" && exit 1)
	@echo "Arquivos principais encontrados."

clean:
	rm -f $(APP) $(TEST_APP) $(APP)_asan *.o run_output_cli.txt test_output.txt

help:
	@echo "Comandos principais:"
	@echo "  make              Compila a CLI"
	@echo "  make run          Executa a CLI interativa"
	@echo "  make demo         Executa roteiro automatico no terminal"
	@echo "  make cli-test     Executa roteiro automatico e salva run_output_cli.txt"
	@echo "  make test         Compila e roda test_scenarios.c, se existir"
	@echo "  make asan         Compila com AddressSanitizer/UBSan e executa"
	@echo "  make check-files  Verifica arquivos esperados"
	@echo "  make clean        Remove binarios e logs"