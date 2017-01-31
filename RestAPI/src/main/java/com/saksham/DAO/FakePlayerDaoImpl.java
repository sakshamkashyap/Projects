package com.saksham.DAO;

import com.saksham.Entity.Player;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Repository;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

@Repository
@Qualifier("fakedata")
public class FakePlayerDaoImpl implements PlayerDao {

    @Autowired
    private static Map<Integer, Player> players;

    static{
        players = new HashMap<Integer, Player>(){
            {
                put(1, new Player(1, "Tom", "Patriots"));
                put(2, new Player(2, "Steph", "Warriors"));
                put(3, new Player(3, "Kershaw", "Dodgers"));
            }
        };
    }

    @Override
    public Collection<Player> getAllPlayers(){
        return this.players.values();
    }

    @Override
    public Player getPlayerById(int id){
        return this.players.get(id);
    }

    @Override
    public void removePlayerById(int id) {
        this.players.remove(id);
    }

    @Override
    public void updatePlayer(Player player){

        Player p = players.get(player.getId());
        p.setTeam(player.getTeam());
        p.setName(player.getName());
        players.put(player.getId(), player);
    }

    @Override
    public void insertPlayer(Player player) {
        this.players.put(player.getId(), player);
    }
}

