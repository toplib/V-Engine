
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

import java.io.File;
import java.util.Objects;

/**
 * Immutable representation of a filesystem resource.
 */
public final class Resource {
    private final ResourceType type;
    private final File file;

    /**
     * Create a new resource.
     *
     * @param type resource type
     * @param file file backing the resource, must not be null
     */
    public Resource(ResourceType type, File file) {
        this.type = Objects.requireNonNull(type, "type");
        this.file = Objects.requireNonNull(file, "file");
    }

    /**
     * Returns the resource type.
     *
     * @return resource type
     */
    public ResourceType getType() {
        return type;
    }

    /**
     * Returns the backing file for this resource.
     *
     * @return file
     */
    public File getFile() {
        return file;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Resource)) return false;
        Resource resource = (Resource) o;
        return type == resource.type && file.equals(resource.file);
    }

    @Override
    public int hashCode() {
        return Objects.hash(type, file);
    }

    @Override
    public String toString() {
        return "Resource{" +
                "type=" + type +
                ", file=" + file +
                '}';
    }
}