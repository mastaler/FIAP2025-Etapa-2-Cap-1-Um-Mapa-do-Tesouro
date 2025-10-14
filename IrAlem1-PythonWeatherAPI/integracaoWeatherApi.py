import requests

API_KEY = "a87d04f6c9d2c97c4bf48f3065e66c9c"
CIDADE = "Sao Paulo, BR"

url = f"https://api.openweathermap.org/data/2.5/weather?q={CIDADE}&appid={API_KEY}&units=metric&lang=pt_br"

resposta = requests.get(url).json()

chuva_prevista = 1 if "rain" in resposta else 0

print("Previsão de chuva:", "Sim" if chuva_prevista else "Não")