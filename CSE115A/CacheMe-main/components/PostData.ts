
import { Geopoint, distanceBetween } from 'geofire-common';
import getLocation from '../helpers/location';
import Location from 'expo-location';
import { collection, addDoc, setDoc, doc, getDoc, updateDoc, GeoPoint, Timestamp } from "firebase/firestore";
import { db, auth, storage } from '../firebase';;
import { getDownloadURL, ref } from "firebase/storage";
import moment from 'moment';

export type CacheData = {
    id: string,
    captionText: string,
    uid: string, 
    image: string,
    numComments: number,
    numLikes: number,
    location: GeoPoint,
    timePosted: Timestamp,
}

export async function getCacheData (postId: string) {
    const cache = {
        __id: "",
        __imageId: "",
        __userId: "",
        _createdAt: new Timestamp(0, 0),
        caption: "",
        comments: [],
        location: new GeoPoint(0, 0),
        numComments: 0,
        numLikes: 0,
        reported: false
    }

    const cacheDoc = await getDoc(doc(db, "cache", postId));

    if (cacheDoc.exists()) {
        cache.__id = postId;
        cache.__imageId = cacheDoc.data().__imageId;
        cache.__userId = cacheDoc.data().__userId;
        cache._createdAt = cacheDoc.data()._createdAt;
        cache.caption = cacheDoc.data().caption;
        cache.location = cacheDoc.data().location;
        cache.comments = cacheDoc.data().comments;
        cache.numComments = cacheDoc.data().numComments;
        cache.numLikes = cacheDoc.data().numLikes;
        cache.reported = cacheDoc.data().reported;
        return cache;
    } else {
        return null;
    }
}

export async function getPoster (uid: string) {
    const docSnap = await getDoc(doc(db, "user", uid));
        if (docSnap.exists()) {
            return docSnap.data().username;
        }
        else {
            return "InvalidUser";
        }
}

export async function getImage (image: string) {
    let uri = "";
    if (image != "" && image != null) {
        const gsRef = ref(storage, "images/" + image);
        await getDownloadURL(gsRef).then( (url) => {
            uri = url;
            return url;
        });
        return uri;
    }  else {
        return uri;
    } 
}

export function getTimeDifference(timePosted: Timestamp) {
    return moment(timePosted.toDate()).fromNow();
}

export async function getDistanceBetween(postLocation: GeoPoint) {
    const loc = await getLocation();
    let currUserLat = loc?.coords.latitude as number;
    let currUserLong = loc?.coords.longitude as number;
    let postLat = postLocation.latitude;
    let postLong = postLocation.longitude;
    const distInBtwn = distanceBetween([currUserLat, currUserLong], [postLat, postLong]);
    return Math.round(distInBtwn);
}

export async function toggleLike (isLiked: boolean, numLikes: number, postId: string) {
    if (isLiked) {
        numLikes += 1
    } else {
        numLikes -= 1
    }
    const cacheRef = doc(db, "cache", postId);
    await updateDoc(cacheRef, {numLikes: numLikes});
    return !isLiked;
}



