package com.saksham.DAO;

import com.saksham.Entity.Player;

import java.util.Collection;

public interface PlayerDao {
    Collection<Player> getAllPlayers();

    Player getPlayerById(int id);

    void removePlayerById(int id);

    void updatePlayer(Player player);

    void insertPlayer(Player player);
}
