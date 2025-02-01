import numpy as np
import sys
import random


REWARDS_STANDARD = {
    '.': -0.1,  # grama
    ';': -0.3,  # grama alta
    '+': -1.0,  # água
    'x': -10.0, # fogo
    'O': 10.0,  # objetivo
    '@': -np.inf # parede
}

REWARDS_POSITIVE = {
    '.': 3.0,
    ';': 1.5,
    '+': 1.0,
    'x': 0.0,
    'O': 10.0,
    '@': -np.inf
}

ACTIONS = {'>': (0, 1), '<': (0, -1), '^': (-1, 0), 'v': (1, 0)}
ACTION_SYMBOLS = list(ACTIONS.keys())
ALPHA = 0.1  # taxa de aprendizado
GAMMA = 0.9  # fator de desconto
EPSILON = 0.1  # exploração ϵ-greedy
STOCHASTIC_PROB = 0.2  # probabilidade de mudança estocástica


def load_map(filename):
    with open(filename, 'r') as f:
        w, h = map(int, f.readline().split())
        grid = [list(f.readline().strip()) for _ in range(h)]
    return np.array(grid), w, h


def initialize_q_table(grid, rewards):
    q_table = {}
    for y in range(grid.shape[0]):
        for x in range(grid.shape[1]):
            if grid[y, x] != '@':
                q_table[(y, x)] = {a: 0 for a in ACTION_SYMBOLS}
    return q_table


def get_next_state(grid, state, action):
    y, x = state
    dy, dx = ACTIONS[action]
    ny, nx = y + dy, x + dx
    
    if ny < 0 or ny >= grid.shape[0] or nx < 0 or nx >= grid.shape[1] or grid[ny, nx] == '@':
        return state  # retorna o mesmo estado se colidir com uma parede ou sair do mapa
    return (ny, nx)


def choose_action(q_table, state, exploration=True):
    if exploration and random.uniform(0, 1) < EPSILON:
        return random.choice(ACTION_SYMBOLS)
    return max(q_table[state], key=q_table[state].get)


def q_learning(grid, start, steps, rewards, stochastic=False):
    q_table = initialize_q_table(grid, rewards)
    state = start
    for _ in range(steps):
        if grid[state] in ['x', 'O']:
            state = start  # reinicia episódio se estado terminal
        
        action = choose_action(q_table, state)
        if stochastic and random.uniform(0, 1) < STOCHASTIC_PROB:
            action = random.choice([a for a in ACTION_SYMBOLS if a != action])
        
        next_state = get_next_state(grid, state, action)
        reward = rewards[grid[next_state]]
        best_next_action = max(q_table[next_state], key=q_table[next_state].get)
        
        q_table[state][action] += ALPHA * (reward + GAMMA * q_table[next_state][best_next_action] - q_table[state][action])
        
        state = next_state
    
    return q_table


def generate_policy(grid, q_table):
    policy = np.copy(grid)
    for state in q_table:
        y, x = state
        if grid[y, x] not in ['x', 'O', '@']:
            policy[y, x] = max(q_table[state], key=q_table[state].get)
    return policy


def main():
    if len(sys.argv) != 6:
        print("Uso: python qlearning.py <map_file> <modification> <start_x> <start_y> <steps>")
        return
    
    map_file = sys.argv[1]
    modification = sys.argv[2]
    start_x, start_y = int(sys.argv[3]), int(sys.argv[4])
    steps = int(sys.argv[5])
    
    grid, w, h = load_map(map_file)
    rewards = REWARDS_STANDARD if modification == "standard" else REWARDS_POSITIVE
    stochastic = modification == "stochastic"
    
    q_table = q_learning(grid, (start_y, start_x), steps, rewards, stochastic)
    policy = generate_policy(grid, q_table)
    
    for row in policy:
        print("".join(row))


if __name__ == "__main__":
    main()