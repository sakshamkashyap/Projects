package com.saksham.DAO;

import com.saksham.Entity.Player;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.Collection;

@Repository
@Qualifier("mongodata")
public class MongoPlayerDaoImpl implements PlayerDao{
    @Override
    public Collection<Player> getAllPlayers() {
        return new ArrayList<Player>(){
            {
                add(new Player(1, "Mario", "Spurs"));
            }
        };
    }

    @Override
    public Player getPlayerById(int id) {
        return null;
    }

    @Override
    public void removePlayerById(int id) {

    }

    @Override
    public void updatePlayer(Player player) {

    }

    @Override
    public void insertPlayer(Player player) {

    }
}
