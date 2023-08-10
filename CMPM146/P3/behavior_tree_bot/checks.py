import logging

def printL(log):
    logging.info(f"{log}")

def if_neutral_planet_available(state): 
    return any(state.neutral_planets())


def have_largest_fleet(state):
    return sum(planet.num_ships for planet in state.my_planets()) \
             + sum(fleet.num_ships for fleet in state.my_fleets()) \
           > sum(planet.num_ships for planet in state.enemy_planets()) \
             + sum(fleet.num_ships for fleet in state.enemy_fleets())


def enemy_fleets_attacking(state):
    for planet in state.my_planets():
        for enemy_fleet in state.enemy_fleets():
            if enemy_fleet.destination_planet == planet.ID:
                return True
    return False
