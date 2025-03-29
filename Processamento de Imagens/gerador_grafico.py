import sys
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import PrecisionRecallDisplay, auc

def calcular_ap(precisoes, revocacoes):
    """Calcula a Average Precision (AP)"""
    indices = np.argsort(revocacoes)
    revocacoes = np.concatenate(([0], revocacoes[indices], [1]))
    precisoes = np.concatenate(([1], precisoes[indices], [0]))
    return auc(revocacoes, precisoes)

def main():
    if len(sys.argv) < 4:
        print("Uso: python script.py <sufixo_titulo> <arquivo_entrada> <arquivo_saida>")
        sys.exit(1)

    try:
        # Carrega e prepara os dados
        data = np.loadtxt(sys.argv[2])
        precisions = data[:, 1]
        recalls = data[:, 2]
        f1_scores = data[:, 3]
        
        # Calcula AP
        ap = calcular_ap(precisions, recalls)
        best_idx = np.argmax(f1_scores)
        
        # Calcula a linha de referência
        if data.shape[1] >= 5:
            nivel_referencia = np.mean(data[:, 4])  # Média da coluna 5 para referência
        else:
            nivel_referencia = np.mean(precisions) * 0.8  # Ajuste empírico
            
        print(f"Valor da linha de referência: {nivel_referencia:.4f}")  # Debug

        # Configuração do gráfico
        plt.figure(figsize=(9, 7))
        ax = plt.gca()

        # 1. Linha de referência (cinza suave)
        plt.axhline(y=nivel_referencia, color='#A0A0A0', linestyle='--', 
                    linewidth=2, alpha=0.8, label=f'Referência (AP = {nivel_referencia:.2f})')

        # 2. Curva principal (azul mais suave)
        disp = PrecisionRecallDisplay(precision=precisions, recall=recalls)
        disp.plot(name=f'Modelo (AP = {ap:.2f})', linewidth=2.5, color='#5F9EA0', ax=ax)

        # 3. Melhor ponto (rosa claro, sem contorno preto)
        plt.scatter(
            recalls[best_idx], precisions[best_idx],
            color='#FFB6C1', s=160, marker='o', edgecolors='none',
            label=f'Melhor F1: {f1_scores[best_idx]:.3f}'
        )

        # Ajustes visuais
        plt.title(f"Curva Precisão-Recall\n({sys.argv[1]})", pad=20, fontsize=14)
        plt.xlabel('Recall', fontsize=12, labelpad=10)
        plt.ylabel('Precisão (Precision)', fontsize=12, labelpad=10)
        
        # Garantir que a linha de referência esteja visível
        y_min = min(0.6, nivel_referencia * 0.9)
        plt.xlim(0.5, 1.03)
        plt.ylim(y_min, 1.03)
        
        ax.grid(True, linestyle=':', alpha=0.3)
        
        # Legenda fora da área do gráfico
        plt.legend(loc='center left', bbox_to_anchor=(1, 0.5), fontsize=11)
        
        # Texto do AP
        plt.figtext(0.5, 0.05, 
                    f"Average Precision (AP) = {ap:.3f}", 
                    ha="center", fontsize=12,
                    bbox={"facecolor":"#F0E68C", "alpha":0.8, "pad":5})
        
        plt.tight_layout(rect=[0, 0.05, 0.85, 1])  # Ajuste para legenda
        plt.savefig(sys.argv[3], dpi=300, bbox_inches='tight')
        print(f"Gráfico finalizado salvo em {sys.argv[3]}")

    except Exception as e:
        print(f"Erro: {str(e)}")
        sys.exit(1)

if __name__ == "__main__":
    main()
