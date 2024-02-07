import random
import csv
#solution for 1.2 Evolving to a target string

#parameters
generations = 100
solutions = 100
string_length = 30
mutation_rate = 0.01

#create a random string of 0s and 1s of length string_length
target_string = [random.randint(0, 1) for _ in range(string_length)]
print("target string:", target_string)

def calculate_fitness(current_String, target_string):
    #calculate fitness for each bit in the current string against the target_strings current bit.
    return sum(current_String[i] == target_string[i] for i in range(len(target_string)))

    #SAME CODE AS GA.py which is explained
def genetic_algorithm(generations, solutions, mutation_rate, target_string):
    population = [[random.randint(0, 1) for _ in range(len(target_string))] for _ in range(solutions)]

    #open cv file
    with open('output_part1.2.csv', 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Generation', 'Average Fitness', 'Best Fitness'])

        #calculate the fitness between the current_string and the target string and generate a fitness score 
        for generation in range(generations):
            fitnesses = [calculate_fitness(current_String, target_string) for current_String in population]
            #find the max or the highest fitness score from the population and save it as best string
            best_string = max(population, key=lambda current_String: calculate_fitness(current_String, target_string))
            best_fitness = calculate_fitness(best_string, target_string)

            #set the new string as new_population and continue with tournament selection to splice and merge
            new_population = [best_string]
            while len(new_population) < solutions:
                parents = [max(random.sample(population, 3), key=lambda ind: calculate_fitness(ind, target_string)) for _ in range(2)]
                point = random.randint(1, len(target_string) - 1)
                offspring = [parents[0][:point] + parents[1][point:], parents[1][:point] + parents[0][point:]]
                for ind in offspring:
                    new_population.append([1 - gene if random.random() < mutation_rate else gene for gene in ind])

            population = new_population[:solutions]
            average_fitness = sum(fitnesses) / solutions

            #write to a new file
            writer.writerow([generation + 1, f"{average_fitness:.2f}", best_fitness])
            print(f"Generation {generation + 1}: Average Fitness = {average_fitness:.2f}, Best String = {best_string}, Best Fitness = {best_fitness}")

            if best_fitness == len(target_string):
                break

genetic_algorithm(generations, solutions, mutation_rate, target_string)
