import sys

def reg(i):
    res = 0 # equiv RA
    if i in ["RB", "01"]: res = 0x100
    elif i in ["RC", "10"]: res = 0x200
    elif i in ["PC", "11"]: res = 0x300
    elif not(i in ["RA", "00"]):
        raise Exception(f"Le mot clé {i} n'est pas reconnu comme un registre.")
    return res

def convert(line : str) -> str :
    instructions = line.split()
    op = instructions[0]
    rd = 0
    if len(instructions) > 1:
        rd = 4*reg(instructions[1])
    binary = 0
    if op == "IMM":     binary = 0x0000 + rd + int(instructions[2], 16)
    elif op == "ADD":   binary = 0x1000 + rd + reg(instructions[2])
    elif op == "ADDI":  binary = 0x2000 + rd + reg(instructions[2]) + int(instructions[3], 16)
    elif op == "SUB":   binary = 0x3000 + rd + reg(instructions[2])
    elif op == "XOR":   binary = 0x4000 + rd + reg(instructions[2])
    elif op == "OR":    binary = 0x5000 + rd + reg(instructions[2])
    elif op == "AND":   binary = 0x6000 + rd + reg(instructions[2])
    elif op == "SR":    binary = 0x7000 + rd
    elif op == "ORI":   binary = 0x8000 + rd + int(instructions[2], 16)
    elif op == "ORIS":  binary = 0x9000 + rd + int(instructions[2], 16)
    elif op == "LD":    binary = 0xA000 + rd + reg(instructions[2])
    elif op == "ST":    binary = 0xB000 + rd + reg(instructions[2])
    elif op == "LT":    binary = 0xC000 + rd + reg(instructions[2]) + int(instructions[3], 16)
    elif op == "EQ":    binary = 0xD000 + rd + reg(instructions[2]) + int(instructions[3], 16)
    elif op == "IO":   binary = 0xE000 + rd + reg(instructions[2])
    elif op == "END":   binary = 0xF000
    else: raise Exception(f"L'instruction {op} n'est pas reconnue.")
    return format(binary, 'X')


if __name__ == "__main__":
    file_name = sys.argv[1]
    if file_name.endswith(".risc"):
        to_write = "v2.0 raw\n"
        try:
            read_file = open(file_name, 'r')
            try:
                for line in read_file.read().split("\n"):
                    instructions, _, _ = line.partition("#")
                    if instructions.replace(" ","") != "":
                        to_write += convert(instructions.upper()) + "\n"

                to_write += convert("END")

                write_name = file_name.replace(".risc",".hex")

                try:
                    write_file = open(write_name, 'w')
                    try:
                        write_file.write(to_write)
                    except:
                        print(f"Erreur lors de l'écriture de {write_name}.")
                    finally:
                        print(f"Le fichier {write_name} a été créé.")
                        write_file.close()
                except:
                    print(f"Erreur lors de la création ou l'ouverture de {write_name}.")

            except Exception as error:
                print(f"Erreur lors de la lecture de {file_name}.\n -> {error}")
            finally:
                read_file.close()
        except:
            print(f"Erreur lors de l'ouverture de {file_name}.")
    else:
        print(f"Le fichier spécifié '{file_name}' n'a pas la bonne extension, ettendue : '.risc'")