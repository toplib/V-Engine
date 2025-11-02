
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
import java.util.Optional;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Central manager for resource loading, caching and unloading.
 */
public final class FileManager {

    private final ConcurrentMap<String, IResourceLoader> extensionLoaderMap = new ConcurrentHashMap<>();
    private final ConcurrentMap<String, CacheEntry> cache = new ConcurrentHashMap<>();

    /**
     * Registers a loader for specific file extensions (without dot), e.g. "vert", "frag", "png".
     *
     * @param loader loader instance, must not be null
     * @param extensions extensions supported by this loader (case-insensitive, without dot)
     */
    public void registerLoader(IResourceLoader loader, String... extensions) {
        Objects.requireNonNull(loader, "loader");
        if (extensions == null || extensions.length == 0) {
            throw new IllegalArgumentException("At least one extension must be provided");
        }
        for (String extRaw : extensions) {
            String ext = normalizeExt(extRaw);
            extensionLoaderMap.put(ext, loader);
        }
    }

    /**
     * Unregisters loader for the given extensions.
     *
     * @param extensions extensions to remove
     */
    public void unregisterLoader(String... extensions) {
        if (extensions == null) return;
        for (String extRaw : extensions) {
            extensionLoaderMap.remove(normalizeExt(extRaw));
        }
    }

    /**
     * Loads resource from path. If already loaded, increments reference count and returns cached resource.
     *
     * @param path path to the resource
     * @return loaded Resource
     * @throws FileSystemException on failure
     */
    public Resource loadResource(String path) throws FileSystemException {
        Objects.requireNonNull(path, "path");
        String normalizedKey = normalizePath(path);

        // fast path: cached
        CacheEntry existing = cache.get(normalizedKey);
        if (existing != null) {
            existing.refCount.incrementAndGet();
            return existing.resource;
        }

        // load with appropriate loader
        IResourceLoader loader = findLoaderForPath(path)
                .orElseThrow(() -> new FileSystemException("No loader registered for path: " + path));

        Resource resource = loader.load(path);
        if (resource == null) {
            throw new FileSystemException("Loader returned null resource for path: " + path);
        }

        CacheEntry inserted = new CacheEntry(resource, new AtomicInteger(1), loader);
        CacheEntry raced = cache.putIfAbsent(normalizedKey, inserted);
        if (raced != null) {
            // someone else inserted concurrently, prefer existing
            raced.refCount.incrementAndGet();
            // unload the one we created to avoid leaking resources (best-effort)
            try {
                loader.unload(resource);
            } catch (Exception ignored) {
            }
            return raced.resource;
        }

        // allow loader to prepare cache (decode data, pin GPU resources, etc)
        loader.cache(resource);
        return resource;
    }

    /**
     * Decrements reference count and unloads the resource when count reaches zero.
     *
     * @param resource resource to unload
     * @throws FileSystemException if resource not managed or unload fails
     */
    public void unloadResource(Resource resource) throws FileSystemException {
        Objects.requireNonNull(resource, "resource");
        String key = normalizePath(resource.getFile().getPath());
        CacheEntry entry = cache.get(key);
        if (entry == null || !entry.resource.equals(resource)) {
            throw new FileSystemException("Resource not managed: " + resource);
        }

        int refs = entry.refCount.decrementAndGet();
        if (refs <= 0) {
            // remove entry only if it's still the same (avoid removing concurrently inserted new one)
            cache.remove(key, entry);
            // delegate to loader to free resource
            entry.loader.unload(resource);
        }
    }

    /**
     * Returns true if resource is present in the cache.
     *
     * @param path resource path
     * @return true if cached
     */
    public boolean isCached(String path) {
        Objects.requireNonNull(path, "path");
        return cache.containsKey(normalizePath(path));
    }

    /**
     * Forces clearing entire cache. Calls loader.unload for each resource.
     */
    public void clearCache() {
        for (CacheEntry entry : cache.values()) {
            try {
                entry.loader.unload(entry.resource);
            } catch (Exception ignored) {
            }
        }
        cache.clear();
    }

    private Optional<IResourceLoader> findLoaderForPath(String path) {
        // first, consult extension map
        String ext = extensionOf(path);
        if (ext != null) {
            IResourceLoader loader = extensionLoaderMap.get(ext);
            if (loader != null) return Optional.of(loader);
        }

        // fallback: ask registered loaders via supports()
        for (IResourceLoader loader : extensionLoaderMap.values()) {
            if (loader.supports(path)) return Optional.of(loader);
        }
        return Optional.empty();
    }

    private static String normalizeExt(String raw) {
        if (raw == null) return "";
        String r = raw.trim();
        if (r.startsWith(".")) r = r.substring(1);
        return r.toLowerCase();
    }

    private static String extensionOf(String path) {
        if (path == null) return null;
        int dot = path.lastIndexOf('.');
        if (dot < 0 || dot == path.length() - 1) return null;
        return path.substring(dot + 1).toLowerCase();
    }

    private static String normalizePath(String path) {
        return new File(path).getPath();
    }

    /**
     * Entry stored in cache: the resource, its refcount and loader.
     */
    private static final class CacheEntry {
        final Resource resource;
        final AtomicInteger refCount;
        final IResourceLoader loader;

        CacheEntry(Resource resource, AtomicInteger refCount, IResourceLoader loader) {
            this.resource = resource;
            this.refCount = refCount;
            this.loader = loader;
        }
    }
}
