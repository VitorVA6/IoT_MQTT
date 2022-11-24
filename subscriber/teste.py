import numpy as np

plot_y = np.array([0.7, 0.5, 0.3, 0.8, 0.5, 0.9, 0.2, 0.8, 0.1, 0.4])
plot_x = np.arange(len(plot_y))
teste = [(x, y) for x, y in zip(plot_x, plot_y)]
print(teste)
