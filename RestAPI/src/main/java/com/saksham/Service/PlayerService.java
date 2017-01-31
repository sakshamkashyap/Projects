package com.saksham.Service;

import com.saksham.DAO.PlayerDao;
import com.saksham.Entity.Player;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;

import java.util.Collection;

@Service
public class PlayerService {

    @Qualifier("fakedata")
    private PlayerDao PlayerDao;

    public Collection<Player> getAllPlayers(){
        return this.PlayerDao.getAllPlayers();
    }

    public Player getPlayerById(int id){
        return this.PlayerDao.getPlayerById(id);
    }

    public void removePlayerById(int id) {
        this.PlayerDao.removePlayerById(id);
    }

    public void updatePlayer(Player player){
        this.PlayerDao.updatePlayer(player);
    }

    public void insertPlayer(Player player) {
        this.PlayerDao.insertPlayer(player);
    }
}
