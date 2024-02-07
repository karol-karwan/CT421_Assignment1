import random
import csv

#solution for 1.3 Deceptive landscape
# Parameters
generations = 100
solutions = 100
string_length = 30
mutation_rate = 0.01

def deceptive_fitness(current_string):
    count = sum(current_string)
    if count == 0:
        #if only 0s present mulitply fitness by 2
        return 2 * string_length
    elif count < string_length / 1:
        #print("test")
        return string_length + count
    else:
        return count


def genetic_algorithm(generations, solutions, string_length, mutation_rate):
    #create a random string of 0s and 1s based on the length of string. In this case 30
    population = [[random.randint(0, 1) for _ in range(string_length)] for _ in range(solutions)]

    #open a CSV file to write to
    with open('output_part1.3.csv', 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Generation', 'Average Fitness', 'Best Fitness'])

        #start going though each generation
        for generation in range(generations):
            fitnesses = [deceptive_fitness(current_String) for current_String in population]
            best_string = max(population, key=deceptive_fitness)
            best_fitness = deceptive_fitness(best_string)
            new_population = [best_string]
            #print(best_string)

            #tournament selection, while loop will keep trying to find a solution until we reach the limit, in this case solutions = 100
            while len(new_population) < solutions:
                #select 2 random parents for a crossover aswell as  2 offspirngs to splice and merge
                parents = [max(random.sample(population, 3), key=sum) for _ in range(2)]
                point = random.randint(1, string_length - 1)
                offspring = [parents[0][:point] + parents[1][point:], parents[1][:point] + parents[0][point:]]
                
                #print(offspring)

                for ind in offspring:
                    # if 1 or 0 is greater than mutation rate, in this case 0.1 we flip the bit of the string. IE 0 -> 1 and vice versa 
                    new_population.append([1 - gene if random.random() < mutation_rate else gene for gene in ind])
            #after the loop update the population with the new population.
            population = new_population[:solutions]
            #calculate the fitness of this new population or generation.
            average_fitness = sum(fitnesses) / solutions

            # Write data to CSV file
            writer.writerow([generation + 1, f"{average_fitness:.2f}"])

            #the loop will keep going until the number of 1s in the string is 30 or whatever the string_length variable is set to.
            if best_fitness == string_length:
                break

genetic_algorithm(generations, solutions, string_length, mutation_rate)
