# /usr/bin/env python
# -*- coding: utf-8 -*-
# Sistema Inicial "ArPI Comunication"
import RPi.GPIO as GPIO
import smbus
import time

# Para RPI versao 1, usar "bus=smbus.SMBus (0)"
bus = smbus.SMBus(1)

# Constantes Globais
const = [1, 2, 3, 4, 5, 6, 7]
HAWKEYE = False

# Ler o dado do dispositivo passando o endereço do mesmo como parametro


def lerDado(adr):
    try:
        numero = bus.read_byte(adr)
    except:
        pass

    return numero

# Atualiza a lista de enreços utilizados e livres


def atualizar():
    address = 8
    listend = []

    while address < 110:
        try:
            bus.write_quick(address)
            listend.append(address)
            time.sleep(0.2)
        except:
            time.sleep(0.02)
            pass
        address = address + 1

    return listend

# Verifica se há um endereço utilizado e retorna uma variavel boolean


def procurarEnd(adr):
    endereco_Indisp = readModules()
    ver = adr in endereco_Indisp

    return ver

# Envia dado para determinado dispositivo, passando o endereçoe o dado


def escreverDado(address, valor):
    numero = bus.write_byte(address, valor)

    return -1

# Verifica numa lista de reservados se determinado elemento está ou nao reservado


def reservado(var):
    # list_res = [-1, 1, 2, 3, 4, 5, 6, 7]
    end_ut = atualizar()

    adr = var in end_ut

    if adr == True:
        r = True
    else:
        r = False

    return r

# Tenta comunicar com um dispositivo


def comunicacao():
    lim = 255
    adr = input("Digite o endereco do disposito(-1 para sair): ")
    if adr != -1:
        ver = procurarEnd(adr)
        if ver == True:
            var = 0
            while var != -1:
                var = input(
                    "Entre com o numero que deseja guardar(-1 para sair): ")
                if 0 < var < lim:
                    numero = 0
                    try:
                        escreverDado(adr, 3)
                        time.sleep(1)
                        numero = lerDado(adr)
                    except:
                        print("Erro! Verifique a conexao.$$1")
                    if numero == 3:
                        try:
                            time.sleep(3)
                            escreverDado(adr, var)
                            print ("~Operacao realizada com sucesso~")
                            time.sleep(2)
                        except:
                            print ("Erro! Verifique a conexao.$$2")
                elif var == -1:
                    print("Menu Inicial. Digite a Opcao desejada.")
                else:
                    print("~Falha na operacao~")
        else:
            print ("Dispostivo nao existe")
            comunicacao()

    return -1

# Reendereçar dispositivo


def reenderecar():
    adr = input("Digite o endereco do disposito que deseja reenderecar: ")
    ver = procurarEnd(adr)

    numero = 0

    if ver == True:
        try:
            escreverDado(adr, 1)
            time.sleep(1)
            numero = lerDado(adr)
        except:
            print ("Erro! Verifique a conexao.")
        if numero == 1:
            print("-----------Reenderecamento----------")
            end = input("Digite o numero do novo endereco: ")
            print("-----------===============----------")
            r = reservado(end)
            # print "r ",r ---?TESTE
            while reservado(end) == True:
                end = input(
                    "Variavel exclusiva para uso interno, digite outra: ")
                reservado(end)

            try:
                time.sleep(3)
                escreverDado(adr, end)
                print ("Aguarde as alteracoes serem efetivadas")
                time.sleep(2)
                print ("Novo Endereco: "), end
            except:
                print ("Erro! Verifique a conexao.")
        else:
            print("Erro no reenderecamento, tentar novamente mais tarte.")
    else:
        print ("Dispostivo nao existe")
        reenderecar()

    return -1

# Recupera um dado guardado em um dispositivo


def recuperarDado():
    adr = input("Digite o endereco do disposito(-1 para Sair): ")
    if adr != -1:
        ver = procurarEnd(adr)
        if ver == True:
            try:
                escreverDado(adr, 2)
                time.sleep(2)
                numero = lerDado(adr)
                time.sleep(2)
                print("Numero recuperado: "), numero
            except:
                print ("Erro! Verifique a conexao.")
        else:
            print ("Dispostivo nao existe")
            recuperarDado()
    else:
        print ("Entre com um comando:> ")

    return -1


def filaProc(request, prd):
    # Prioridade: 1 - Alta
    #            2 - Media
    #            3 - Baixa

    if prd == 1:
        if request == "e":
            print ("Processo de escrita ativo(prd1)")
            reenderecar()
    if prd == 2:
        if request == "c":
            print ("Inserir processo")
    if prd == 3:
        if request == "l":
            print("Processo de leitura ativo!(prd3)")
            lista = readModules()
            print ((lista))
        if request == "ld":
            print ("Processo de leitura de dado ativo(prd3)")
            recuperarDado()
        if request == "e":
            print ("Processo de comunicacao ativo!(prd3)")
            comunicacao()

    return -1


def controlador(request, prd):

    if not GPIO.event_detected(23):
        if request == "e":
            filaProc(request, prd)
        elif request == "c":
            filaProc(request, prd)
        elif request == "l":
            filaProc(request, prd)
        elif request == "ld":
            filaProc(request, prd)
        eye = True
    else:
        eye = False

    return eye


def loadModules():
    # carrega a lista de enderecos e converte a lista de inteiro para string
    lista_end = atualizar()
    lista_str = []
    i = 0

    while i < len(lista_end):
        aux = lista_end[i]
        # print type(aux)
        cnv = str(aux)
        lista_str.append(cnv)
        # print type(cnv)
        i = i + 1

    try:
        arq = open('/tmp/listaEnd.txt', 'w')
        arq.writelines(lista_str)
        arq.close()
    except:
        pass

    time.sleep(1)
    return -1


def readModules():
    # Faz a leitura da lista de enderecos e converte a lista de string para inteiro
    try:
        arq = open('/tmp/listaEnd.txt', 'r')
        lista = arq.readlines()
    except:
        pass

    i = 0
    listar = []
    while i < len(lista):
        listar.append(int(lista[i]))
        i = i + 1

    return listar


def stateSave(dados):
    # Salva o estado atual do programa
    d = str(dados)

    try:
        arq = open('/tmp/StateSave.txt', 'w')
        arq.writelines(d)
        arq.close()
    except:
        pass

    return -1


def stateReturn():
    # Retorna o ultimo estado salvo do programa
    try:
        arq = open('/tmp/StateSave.txt', 'r')
        dado = arq.readlines()
    except:
        dado = "0"

    aux = int(dado[0])
    if not aux:
        aux = 0

    return aux


def hawkeye():
    # Procura o dispositivo que pediu a interrupcao
    lista_dispositivos = readModules()
    i = 0
    fail = 0
    while i < len(lista_dispositivos):
        endereco = lista_dispositivos[i]
        if procurarEnd(endereco) == True:
            choice = 0
            try:
                escreverDado(endereco, 4)
                time.sleep(1)
                choice = lerDado(endereco)
            except:
                print ("Erro na transicao dos pacotes")
                pass
            if choice == 5:
                try:
                    escreverDado(endereco, 5)
                except:
                    pass
            elif choice == 4:
                try:
                    escreverDado(endereco, 4)
                except:
                    pass
        else:
            fail = fail + 1
        time.sleep(2)
        i = i + 1
    return fail

# Menu Principal


def principal():

    print("Preparando sistema de interrupcao...")
    interruptModule()

    alta = 1
    media = 2
    baixa = 3

    print ("------------------Menu------------------")
    print ("Digite a opcao desejada!")
    print ("1 - Atualizar a lista de dispositivos")
    print ("2 - Ver a lista de enderecos utilizados.")
    print ("3 - Abrir canal de comunicacao.")
    print ("4 - Recuperar dados do dispositivo.")
    print ("5 - Reenderecar um dispositivo.")
    print ("0 - Sair")

    while True:
        teste()
        itp = stateReturn()
        if itp == 0:
            aux = input(":>>")
        else:
            aux = itp
            stateSave(0)
        if GPIO.event_detected(23):
            print ("Interrupcao em andamento")
            print ("Salvando estado atual")
            stateSave(aux)
            time.sleep(2)
            print ("Salvo!! Prosseguindo com interrupcao")
            fail = hawkeye()
            print ("Falhas: "), fail
            time.sleep(2)
            print ("Retornando ao estado anterior a interrupcao\n")
        else:
            # Prioridade: 1 - Alta
            #            2 - Media
            #            3 - Baixa
            if aux == 1:
                loadModules()
                print ("Atualizado com Sucesso")
            elif aux == 2:
                controlador("l", baixa)
            elif aux == 3:
                controlador("e", baixa)
            elif aux == 4:
                controlador("ld", baixa)
            elif aux == 5:
                controlador("e", alta)
                loadModules()
            elif aux == 0:
                break
            else:
                print ("Digite um comando valido")

    return -1


def teste():
    if GPIO.input(23) == 1:
        print ("Taxa alta")
    else:
        print ("Taxa baixa")

# ------------------------------------------------------------------------------#


def interruptModule():
    # INTERRUPCAO#

    GPIO.setmode(GPIO.BCM)

# GPIO 23 set up as input. It is pulled up to stop false signals
    GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_UP)

# raw_input("Press Enter when ready\n>")

    try:
        GPIO.add_event_detect(23, GPIO.FALLING)
    except KeyboardInterrupt:
        print("Sistema de interrupcao nao detectado.")
        GPIO.cleanup()       # clean up GPIO on CTRL+C exit
        quit()

    time.sleep(1)


# ------------------------------------------------------------------------------#
print("Carregando os modulos de operacao. Aguarde...")
loadModules()
principal()
GPIO.cleanup()
