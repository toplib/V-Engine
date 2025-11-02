
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

package org.engine.vengine.filesystem;

/**
 * Loader responsible for loading and unloading resources.
 */
public interface IResourceLoader {

    /**
     * Loads resource located at the provided path.
     *
     * @param path file path
     * @return loaded Resource
     * @throws FileSystemException if resource cannot be loaded
     */
    Resource load(String path) throws FileSystemException;

    /**
     * Unloads the given resource from memory.
     *
     * @param resource resource to unload
     * @throws FileSystemException if resource cannot be unloaded or doesn't exist
     */
    void unload(Resource resource) throws FileSystemException;

    /**
     * Called when resource should be cached/pinned in memory.
     * Implementation may use this to keep decoded data in memory or increase internal counters.
     *
     * @param resource resource to cache
     */
    default void cache(Resource resource) {
        // default no-op
    }

    /**
     * Returns true if this loader supports the resource path (by extension or other rule).
     *
     * @param path resource path
     * @return true if supported
     */
    boolean supports(String path);
}