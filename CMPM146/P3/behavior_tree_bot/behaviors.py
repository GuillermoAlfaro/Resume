import sys
sys.path.insert(0, '../')
from planet_wars import issue_order

import logging

def printL(log):
    logging.info(f"{log}")

def predict_ships(state,pt_a,pt_b):
    return state.distance(pt_a.ID, pt_b.ID) * pt_b.growth_rate + 1

def attack_weakest_enemy_planet(state):
    # (1) If we currently have a fleet in flight, abort plan.
    if len(state.my_fleets()) >= 1:
        return False

    # (2) Find my strongest planet.
    strongest_planet = max(state.my_planets(), key=lambda t: t.num_ships, default=None)

    # (3) Find the weakest enemy planet.
    weakest_planet = min(state.enemy_planets(), key=lambda t: t.num_ships, default=None)

    if not strongest_planet or not weakest_planet:
        # No legal source or destination
        printL("ATTACK WEAKEST ENEMY PLANET FAILED")
        return False
    else:
        # (4) Send half the ships from my strongest planet to the weakest enemy planet
        # 
        # TODO: Make sure strongest planet has enough ships !!
        printL("ATTACK WEAKEST ENEMY PLANET SUCCESS")
        return issue_order(state, strongest_planet.ID, weakest_planet.ID, weakest_planet.num_ships + predict_ships(state,strongest_planet,weakest_planet))

def attack_best(state):
    my_planets = iter(sorted(state.my_planets(), key=lambda p: p.num_ships))

    enemy_planets = [planet for planet in state.enemy_planets()
                      if not any(fleet.destination_planet == planet.ID for fleet in state.my_fleets())]
    enemy_planets.sort(key=lambda p: p.num_ships)

    target_planets = iter(enemy_planets)

    try:
        my_planet = next(my_planets)
        target_planet = next(target_planets)
        while True:
            required_ships = target_planet.num_ships + \
                                 state.distance(my_planet.ID, target_planet.ID) * target_planet.growth_rate + 2

            if my_planet.num_ships > required_ships:
                issue_order(state, my_planet.ID, target_planet.ID, required_ships)
                my_planet = next(my_planets)
                target_planet = next(target_planets)
            else:
                my_planet = next(my_planets)

    except StopIteration:
        return

def get_closest_planet(state):
    # (1) If we currently have a fleet in flight, just do nothing.
    if len(state.my_fleets()) >= 1:
        return False

    # (2) Find my strongest planet.
    strongest_planet = max(state.my_planets(), key=lambda p: p.num_ships, default=None)
    
    # (3) Sort planets by distance
    planets_by_distance = sorted(state.neutral_planets(), key=lambda p: state.distance(p.ID, strongest_planet.ID), reverse=False)

    # (3.1) Cut list to closest 1 planets
    del planets_by_distance[1:]

    # (3.1) Get the planet with the lowest fleet
    close_weak_planet = min(planets_by_distance, key=lambda p: p.num_ships, default = None)
    
    if not strongest_planet or not close_weak_planet:
        # No legal source or destination
        return False
    else:
        # (4) Send half the ships from my strongest planet to the weakest enemy planet.
        return issue_order(state, strongest_planet.ID, close_weak_planet.ID, close_weak_planet.num_ships + 1)
        
def spread_neutral(state):

    my_planets = iter(sorted(state.my_planets(), key=lambda p: p.num_ships))

    neutral_planets = [planet for planet in state.neutral_planets()
                      if not any(fleet.destination_planet == planet.ID for fleet in state.my_fleets())]
    neutral_planets.sort(key=lambda p: p.num_ships)

    target_planets = iter(neutral_planets)

    try:
        my_planet = next(my_planets)
        target_planet = next(target_planets)
        while True:
            required_ships = target_planet.num_ships + 2

            if my_planet.num_ships > required_ships:
                issue_order(state, my_planet.ID, target_planet.ID, required_ships)
                my_planet = next(my_planets)
                target_planet = next(target_planets)
            else:
                my_planet = next(my_planets)

    except StopIteration:
        return
    

def defend_attacked_planet(state):
    
    # (1) If we currently have a fleet in flight, just do nothing.
    # if len(state.my_fleets()) >= 1:
    #     return False
    
    # (2) Find my strongest planet.
    strongest_planet = max(state.my_planets(), key=lambda t: t.num_ships, default=None)

    attacked_planet = None
    # (3) Find the attacked planet.
    for planet in state.my_planets():   
        for enemy in state.enemy_fleets():
            if enemy.destination_planet == planet.ID:
                attacked_planet = planet
    
    if not strongest_planet or attacked_planet:
        # No strongest_planet or attacked_planet
        return False
    else:
        # Defend planet being attacked with strongest planet's fleet
        return issue_order(state, strongest_planet, attacked_planet.ID, attacked_planet.num_ships + predict_ships(state,strongest_planet,attacked_planet))

def defend_optimized(state):

    my_planets = sorted(state.my_planets(), key=lambda t: t.num_ships, reverse=True)
    strongest_planet = my_planets[0]
    second_strongest_planet = my_planets[1] if len(my_planets) > 1 else None


    #strongest_planet = second_strongest_planet

    

    attacked_planet = None
    attacking_fleet = None
    
    for planet in state.my_planets():   
        for enemy_fleet in state.enemy_fleets():
            if enemy_fleet.destination_planet == planet.ID:
                attacked_planet = planet
                attacking_fleet = enemy_fleet

    
    
    if not strongest_planet or not attacked_planet: # No strongest_planet or attacked_planet
        return False
    else:   # Defend planet being attacked with strongest planet's fleet
        return issue_order(state, strongest_planet.ID, attacked_planet.ID, enemy_fleet.num_ships)
