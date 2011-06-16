/*
 * HLZoneInfoProvider.h
 * 
 * Horologe
 * Copyright (c) 2010 Pilgrimage Software
 *
 * A Cocoa version of the Joda-Time Java date/time library.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import <Foundation/Foundation.h>

#import "HLProvider.h"

 
@interface HLZoneInfoProvider : HLProvider {

    @private
    /** The directory where the files are held. */
//    private final File iFileDir;
    /** The resource path. */
NSString* iResourcePath;
    /** The class loader to use. */
//    private final ClassLoader iLoader;
    /** Maps ids to strings or SoftReferences to DateTimeZones. */
    NSDictionary* iZoneInfoMap;    
}

// TODO: this one is tricksy, since it uses Java-specific classes, like class loaders and input classes
// will require some research to port
#if 0
/**
 */
public ZoneInfoProvider(File fileDir) throws IOException {
    if (fileDir == null) {
        throw new IllegalArgumentException("No file directory provided");
    }
    if (!fileDir.exists()) {
        throw new IOException("File directory doesn't exist: " + fileDir);
    }
    if (!fileDir.isDirectory()) {
        throw new IOException("File doesn't refer to a directory: " + fileDir);
    }
    
    iFileDir = fileDir;
    iResourcePath = null;
    iLoader = null;
    
    iZoneInfoMap = loadZoneInfoMap(openResource("ZoneInfoMap"));
}

/**
 * ZoneInfoProvider searches the given ClassLoader resource path for
 * compiled data files. Resources are loaded from the ClassLoader that
 * loaded this class.
 *
 * @throws IOException if directory or map file cannot be read
 */
public ZoneInfoProvider(String resourcePath) throws IOException {
    this(resourcePath, null, false);
}

/**
 * ZoneInfoProvider searches the given ClassLoader resource path for
 * compiled data files.
 *
 * @param loader ClassLoader to load compiled data files from. If null,
 * use system ClassLoader.
 * @throws IOException if directory or map file cannot be read
 */
public ZoneInfoProvider(String resourcePath, ClassLoader loader)
throws IOException
{
    this(resourcePath, loader, true);
}

/**
 * @param favorSystemLoader when true, use the system class loader if
 * loader null. When false, use the current class loader if loader is null.
 */
private ZoneInfoProvider(String resourcePath,
                         ClassLoader loader, boolean favorSystemLoader) 
throws IOException
{
    if (resourcePath == null) {
        throw new IllegalArgumentException("No resource path provided");
    }
    if (!resourcePath.endsWith("/")) {
        resourcePath += '/';
    }
    
    iFileDir = null;
    iResourcePath = resourcePath;
    
    if (loader == null && !favorSystemLoader) {
        loader = getClass().getClassLoader();
    }
    
    iLoader = loader;
    
    iZoneInfoMap = loadZoneInfoMap(openResource("ZoneInfoMap"));
}

//-----------------------------------------------------------------------
/**
 * If an error is thrown while loading zone data, uncaughtException is
 * called to log the error and null is returned for this and all future
 * requests.
 * 
 * @param id  the id to load
 * @return the loaded zone
 */
public synchronized DateTimeZone getZone(String id) {
    if (id == null) {
        return null;
    }
    
    Object obj = iZoneInfoMap.get(id);
    if (obj == null) {
        return null;
    }
    
    if (id.equals(obj)) {
        // Load zone data for the first time.
        return loadZoneData(id);
    }
    
    if (obj instanceof SoftReference<?>) {
        @SuppressWarnings("unchecked")
        SoftReference<DateTimeZone> ref = (SoftReference<DateTimeZone>) obj;
        DateTimeZone tz = ref.get();
        if (tz != null) {
            return tz;
        }
        // Reference cleared; load data again.
        return loadZoneData(id);
    }
    
    // If this point is reached, mapping must link to another.
    return getZone((String)obj);
}

/**
 * Gets a list of all the available zone ids.
 * 
 * @return the zone ids
 */
public synchronized Set<String> getAvailableIDs() {
    // Return a copy of the keys rather than an umodifiable collection.
    // This prevents ConcurrentModificationExceptions from being thrown by
    // some JVMs if zones are opened while this set is iterated over.
    return new TreeSet<String>(iZoneInfoMap.keySet());
}

/**
 * Called if an exception is thrown from getZone while loading zone data.
 * 
 * @param ex  the exception
 */
protected void uncaughtException(Exception ex) {
    Thread t = Thread.currentThread();
    t.getThreadGroup().uncaughtException(t, ex);
}

/**
 * Opens a resource from file or classpath.
 * 
 * @param name  the name to open
 * @return the input stream
 * @throws IOException if an error occurs
 */
private InputStream openResource(String name) throws IOException {
    InputStream in;
    if (iFileDir != null) {
        in = new FileInputStream(new File(iFileDir, name));
    } else {
        String path = iResourcePath.concat(name);
        if (iLoader != null) {
            in = iLoader.getResourceAsStream(path);
        } else {
            in = ClassLoader.getSystemResourceAsStream(path);
        }
        if (in == null) {
            StringBuffer buf = new StringBuffer(40)
            .append("Resource not found: \"")
            .append(path)
            .append("\" ClassLoader: ")
            .append(iLoader != null ? iLoader.toString() : "system");
            throw new IOException(buf.toString());
        }
    }
    return in;
}

/**
 * Loads the time zone data for one id.
 * 
 * @param id  the id to load
 * @return the zone
 */
private DateTimeZone loadZoneData(String id) {
    InputStream in = null;
    try {
        in = openResource(id);
        DateTimeZone tz = DateTimeZoneBuilder.readFrom(in, id);
        iZoneInfoMap.put(id, new SoftReference<DateTimeZone>(tz));
        return tz;
    } catch (IOException e) {
        uncaughtException(e);
        iZoneInfoMap.remove(id);
        return null;
    } finally {
        try {
            if (in != null) {
                in.close();
            }
        } catch (IOException e) {
        }
    }
}

//-----------------------------------------------------------------------
/**
 */
private static Map<String, Object> loadZoneInfoMap(InputStream in) throws IOException {
    Map<String, Object> map = new TreeMap<String, Object>(String.CASE_INSENSITIVE_ORDER);
    DataInputStream din = new DataInputStream(in);
    try {
        readZoneInfoMap(din, map);
    } finally {
        try {
            din.close();
        } catch (IOException e) {
        }
    }
    map.put("UTC", new SoftReference<DateTimeZone>(DateTimeZone.UTC));
    return map;
}

/**
 */
+ (void)readZoneInfoMap:(DataInputStream din, Map<String, Object> zimap) throws IOException {
    // Read the string pool.
    int size = din.readUnsignedShort();
    String[] pool = new String[size];
    for (int i=0; i<size; i++) {
        pool[i] = din.readUTF().intern();
    }
    
    // Read the mappings.
    size = din.readUnsignedShort();
    for (int i=0; i<size; i++) {
        try {
            zimap.put(pool[din.readUnsignedShort()], pool[din.readUnsignedShort()]);
        } catch (ArrayIndexOutOfBoundsException e) {
            throw new IOException("Corrupt zone info map");
        }
    }
}
#endif

@end
