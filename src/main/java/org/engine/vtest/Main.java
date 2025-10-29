/*
 * V-Engine
 * Copyright (C) 2025
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package org.engine.vtest;

import org.engine.vengine.boot.Engine;
import org.engine.vengine.boot.EngineConfig;
import org.engine.vengine.logging.LogLevel;
import org.engine.vengine.logging.LogManager;
import org.engine.vengine.logging.Logger;
import org.engine.vengine.logging.LoggerConfig;

public class Main {
    private static final Logger logger = LogManager.get().getLogger(Main.class);

    public static void main(String[] args){
        LoggerConfig cfg = new LoggerConfig()
                .setLevel(LogLevel.DEBUG)
                .addConsole()
                .addRollingFile("logs/engine.log", 10 * 1024 * 1024L, 5)
                .setAsync(false)
                .setQueueCapacity(32768);

        LogManager.get().applyConfig(cfg);

        logger.info("Engine test startup...");
        logger.debug("Logger initialized successfully.");
        //try { Thread.sleep(100); } catch (InterruptedException ignored) {}
        
        LogManager.get().flush();
        EngineConfig config = new EngineConfig();
        config.setAppName("Test Application");
        config.setDebug(true);
        config.setVersion("Alpha 0.0.1");
        config.setWidth(800);
        config.setHeight(600);

        Engine engine = new Engine();
        engine.create(config);
        engine.start();

        LogManager.get().shutdown();
    }
}
