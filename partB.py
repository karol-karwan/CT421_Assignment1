import random
import csv
#solution for part b. 

solutions = 50
generations = 50
mutation_rate = 0.1
BPP_instances = [
 #[46,1000,200,200,200,199,198,198,197,197,194,194,193,192,191,191,191,190,190,189,188,188,187,187,186,185,185,185,185,184,184,184,183,183,183,182,182,182,181,181,180,179,179,179,179,178,177,177,177,177,175,174,173,173,172,171,171,171,170,170,169,169,169,167,167,165,165,164,163,163,163,163,162,161,160,160,159,158,158,158,157,156,156,156,156,156,156,155,155,155,154,154,153,152,152,152,151,151,150,150,150,150],
 #[47,1000,200,200,199,199,199,199,198,197,196,196,195,195,194,194,193,191,191,190,189,189,188,187,187,186,185,185,184,184,184,184,184,183,182,181,181,181,180,180,179,179,178,176,175,175,174,174,174,174,174,173,172,172,172,171,170,170,170,170,169,169,168,167,167,167,167,167,165,165,164,164,163,163,163,162,162,160,160,159,159,158,158,157,157,157,157,156,156,156,155,155,154,153,153,153,152,152,151,151,150,150],
 #[44,1000,200,199,199,197,197,196,196,193,193,193,192,192,191,191,190,190,189,189,189,188,187,185,185,185,183,183,182,181,181,181,180,180,180,179,179,179,178,177,177,177,177,177,176,176,175,175,175,175,175,174,174,174,174,173,171,171,171,170,169,169,168,168,168,168,168,167,166,166,166,166,165,165,163,162,162,161,161,160,160,160,159,159,158,158,157,156,156,156,155,155,155,154,153,153,152,152,152,151,151,150],
 #[42,1000,200,200,200,199,199,199,199,199,198,198,198,198,197,195,193,193,193,193,192,188,187,186,186,186,185,185,185,184,184,183,183,182,181,180,180,180,179,179,178,178,178,178,178,178,177,177,176,176,176,176,175,173,173,173,173,172,172,172,172,170,169,169,169,168,168,168,168,167,165,165,165,164,163,163,162,162,162,162,161,160,160,160,159,159,159,158,157,157,157,155,155,154,154,154,153,152,152,152,151,150],
 [44,1000,200,200,200,200,200,199,199,198,198,197,197,196,195,195,195,194,194,193,193,192,192,192,192,191,191,190,190,190,190,188,188,188,187,187,186,186,185,184,183,182,181,181,181,180,178,178,178,177,177,176,176,174,173,172,171,171,171,168,168,167,165,164,163,162,162,162,161,161,161,160,160,160,159,159,158,158,158,157,157,157,156,156,155,155,155,155,155,154,154,154,153,153,153,151,151,151,151,151,150,150]
]

def fitness(solution, items, bin_capacity):
    bin_weights = [0] * (max(solution) + 1)
    penalty1 = 0 #penalty for overflowing the bins if that happens
    penalty2 = 0  #penalty for how much space we takee up

    for item_index, bin_index in enumerate(solution):
        bin_weights[bin_index] += items[item_index]

        if bin_weights[bin_index] > bin_capacity:
            penalty1 += (bin_weights[bin_index] - bin_capacity)  #more penalty based on overflow of the bins

        #if the current item in the bin has a weight thats greater than the bin capacity we want to add a penalty
        if bin_weights[bin_index] > bin_capacity * 2:  
            penalty2 += bin_weights[bin_index] - bin_capacity * 2

    #get penalty for both the overflow and how much of the bin was used
    total_penalty = penalty1 * 0.5 + penalty2 * 0.5
    #return the value 
    return -total_penalty if total_penalty else -len(set(solution)) 

def run_genetic_algorithm(BPP_instances):
   #open a csv file. I didnt know how to automate it so I just changed it from BPP1 -> BPP5.
    with open('output_partb_BPP5.csv', 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Generation','Best Fitness'])

    #1 of 5 bpp in the .txt file. This code is pretty similar to GA1.1 to 1.3.py 
    for BPP in BPP_instances:
        _, bin_capacity, *items = BPP
        num_bins = max(len(items), 50)  

        population = [[random.randint(0, num_bins-1) for _ in range(len(items))] for _ in range(solutions)]

        for generation in range(generations):
            fitnesses = [fitness(ind, items, bin_capacity) for ind in population]
            new_population = []

            #tournament selection, while loop will keep trying to find a solution until we reach the limit, in this case solutions = 100
            while len(new_population) < solutions:
                parents = random.choices(population, weights=[f - min(fitnesses) + 1 for f in fitnesses], k=2)
                offspring1, offspring2 = parents[0][:], parents[1][:] 
                point = random.randint(1, len(items) - 1)
                offspring1[point:], offspring2[point:] = offspring2[point:], offspring1[point:]

                for offspring in (offspring1, offspring2):
                    if random.random() < mutation_rate:
                        offspring[random.randint(0, len(items)-1)] = random.randint(0, num_bins-1)
                    new_population.append(offspring)

            population = new_population[:solutions]
            best_fitness = max(fitnesses)
            #print(best_fitness)
            with open('output_partb_BPP5.csv', 'a', newline='') as file:
                writer = csv.writer(file)
                writer.writerow([generation + 1, best_fitness])

run_genetic_algorithm(BPP_instances)