import subprocess
import os, sys
import random
from datetime import datetime
random.seed(datetime.now().timestamp())

wins = 0
production_win = 0
agro_win = 0
spread_win = 0
defence_win = 0
easy_win = 0

prod = []
agro = []
spread = []
defense = []



def show_match(bot, opponent_bot, map_num):
    """
        Runs an instance of Planet Wars between the two given bots on the specified map. After completion, the
        game is replayed via a visual interface.
    """
    command = 'java -jar tools/PlayGame.jar maps/map' + str(map_num) + '.txt 1000 1000 log.txt ' + \
              '"python ' + bot + '" ' + \
              '"python ' + opponent_bot + '" ' + \
              '| java -jar tools/ShowGame.jar'
    print(command)
    os.system(command)

def test(bot, opponent_bot, map_num):
    """ Runs an instance of Planet Wars between the two given bots on the specified map. """
    bot_name, opponent_name = bot.split('/')[1].split('.')[0], opponent_bot.split('/')[1].split('.')[0]
    #print('Running test:',bot_name,'vs',opponent_name)
    command = 'java -jar tools/PlayGame.jar maps/map' + str(map_num) +'.txt 1000 1000 log.txt ' + \
              '"python ' + bot + '" ' + \
              '"python ' + opponent_bot + '" '
    #print(command)
    p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
    global wins
    global production_win
    global agro_win
    global spread_win
    global defence_win
    global easy_win
    global prod
    global agro
    global spread
    global defense


    while True:
        return_code = p.poll()  # returns None while subprocess is running
        line = p.stdout.readline().decode('utf-8')
        if '1 timed out' in line:
            print(bot_name,'timed out.')
            break
        elif '2 timed out' in line:
            print(opponent_name,'timed out.')
            break
        elif '1 crashed' in line:
            print(bot_name, 'crashed.')
            break
        elif '2 crashed' in line:
            print(opponent_name, 'crashed')
            break
        elif 'Player 1 Wins!' in line:
            #print(bot_name,'wins!')
            wins += 1
            break
        elif 'Player 2 Wins!' in line:
            if opponent_name == "production_bot":
                production_win += 1
                print(opponent_name,'wins!', production_win)
                prod.append(map_num)
            elif opponent_name == "aggressive_bot":
                agro_win += 1
                print(opponent_name,'wins!', agro_win)
                agro.append(map_num)
            elif opponent_name == "spread_bot":
                spread_win += 1
                print(opponent_name,'wins!', spread_win)
                spread.append(map_num)
            elif opponent_name == "defensive_bot":
                defence_win += 1
                print(opponent_name,'wins!', defence_win)
                defense.append(map_num)
            elif opponent_name == "easy.bot":
                easy_win += 1
            break

        if return_code is not None:
            break


if __name__ == '__main__':
    path =  os.getcwd()
    # opponents = ['opponent_bots/easy_bot.py',
    #               'opponent_bots/spread_bot.py',
    #               'opponent_bots/aggressive_bot.py',
    #               'opponent_bots/defensive_bot.py',
    #               'opponent_bots/production_bot.py']
    

    # test_about = 20
    # opponents *= test_about
    # maps = []
    # for x in range(100):
    #     #maps.append(random.randint(0, 99))
    #     maps.append(x)

    opponents = []

    
    for _ in range(100):
        opponents.append("opponent_bots/spread_bot.py")
    for _ in range(100):
        opponents.append("opponent_bots/aggressive_bot.py")
    for _ in range(100):
        opponents.append("opponent_bots/defensive_bot.py")
    for _ in range(100):
        opponents.append("opponent_bots/production_bot.py")
    maps = []

    for x in range(100):
        maps.append(x)
    maps *= 4



    my_bot = 'behavior_tree_bot/bt_bot.py'
    # show = len(sys.argv) < 2 or sys.argv[1] == "show"
    show = False
    for opponent, map in zip(opponents, maps):
        # use this command if you want to observe the bots
        if show:
            show_match(my_bot, opponent, map)
        else:
            # use this command if you just want the results of the matches reported
            test(my_bot, opponent, map)
    
    print("Our winrate against Spread bot:", 1 - spread_win/100, spread)        
    print("Our winrate against Aggressive_bot:", 1 - agro_win/100, agro)
    print("Our winrate against Defense bot:", 1 - defence_win/100, defense)
    print("Our winrate against Production bot:", 1 - production_win/100, prod)

    
    #print("Our winrate against Easy bot :", 1 - easy_win/100)
