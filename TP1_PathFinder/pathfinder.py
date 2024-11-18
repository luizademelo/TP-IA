import sys 

def parse_input(): 

    if len(sys.argv) != 7: 
        print("Usage: python3 pathfinder.py [caminho_para_arquivo_mapa] [identificador_metodo] xi yi xf yf")
        sys.exit(1)

    caminho_arquivo = sys.argv[1]
    metodo = sys.arv[2]
    inicio = (int(sys.argv[3]), int(sys.argv[4]))  # Tuple (xi, yi)
    fim = (int(sys.argv[5]), int(sys.argv[6]))    # Tuple (xf, yf)
    
    
    return caminho_arquivo, metodo, inicio, fim

if __name__ == "__main__": 

    caminho_arquivo, metodo, inicio, fim = parse_input()

    if metodo == "UCS": 
        pass