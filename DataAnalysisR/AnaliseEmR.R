library(readr)
library(ggplot2)

# Definir o diretorio do arquivo R para o que tiver o arquivo .csv
setwd("D:/FIAP/")

# Obter esse arquivo e ler utilizando o readr com todos os registros importados e gerados pelo programa do Esp32
dados <- read_csv("dadosESP32FarmTech.csv")
dados$id <- seq_along(dados$umidadeSimulada)

# Calculo da media_umidade 
media_umidade <- mean(dados$umidadeSimulada)

# Calculo da mediana Ph
mediana_ph <- median(dados$phSimulado)

# Calculo do desvio Padrão ph
desvio_padrao_ph <- sd(dados$phSimulado)

# Caculo Regressão linear ph e umidade
modelo_ph_umid <- lm(umidadeSimulada ~ phSimulado, data = dados)
summary(modelo_ph_umid)


print(paste("Média de Umidade:", media_umidade))
print(paste("Mediana pH:", mediana_ph))
print(paste("Desvio Padrão pH:", desvio_padrao_ph))

ggplot(dados, aes(x = id, y = umidadeSimulada)) +
  geom_line(color = "steelblue", size = 1.2) +
  geom_point(color = "blue", size = 1.5) +
  geom_hline(yintercept = 60, color = "red", linetype = "dashed", size = 1) +
  geom_hline(yintercept = 80, color = "darkgreen", linetype = "dashed", size = 1) +
  geom_hline(yintercept = media_umidade, color = "deepskyblue", linetype = "dotdash", size = 1.2) +
  annotate("text", x = max(dados$id) * 0.9, y = 60, label = "Ligar Bomba (60)", color = "red", vjust = -1) +
  annotate("text", x = max(dados$id) * 0.9, y = 80, label = "Desligar Bomba (80)", color = "darkgreen", vjust = -1) +
  annotate("text", x = max(dados$id) * 0.9, y = media_umidade, label = paste0("Média (", round(media_umidade, 1), ")"), color = "deepskyblue", vjust = -1) +
  labs(
    title = "Evolução da Umidade",
    x = "Tempo",
    y = "Umidade"
  ) +
  theme_minimal(base_size = 14)

