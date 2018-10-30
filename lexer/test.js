import { curry, Task } from "fp-small";

/**
 * Sets Data
 * setData :: Ref -> id -> (data,merge) -> Task Ref
 * @param {docRef} docRef - DocRef to be set
 * @param {object} data - Data to be set
 * @param {boolean} merge - Merge Data ?
 * @returns {Task} - fork (rej(err),res())
 */
export const setDoc = curry(
  (docRef, data, ...merge) =>
    new Task((rej, res) =>
      docRef
        .set(data, { merge: Boolean(merge.length && merge[0]) })
        .then(res)
        .catch(rej)
    )
);

/**
 * Data with auto generated ID
 * addDoc :: Ref -> {data} -> Task
 * @param {collectionRef} collection - Parent Collection
 * @param {object} data - Data to be added
 * @returns {Task} - fork (rej(err),res(docRef))
 */
export const addDoc = curry(
  (collection, data) =>
    new Task((rej, res) =>
      collection
        .add(data)
        .then(res)
        .catch(rej)
    )
);

/**
 * Update Document
 * updateDoc :: Ref -> id -> data -> Task Ref
 * @param {docRef} docRef - DocRef to be updated
 * @param {object} data - Data to be updated
 * @returns {Task} - fork (rej(err),res())
 */
export const updateDoc = curry(
  (docRef, data) =>
    new Task((rej, res) =>
      docRef
        .update(data)
        .then(res)
        .catch(rej)
    )
);

/**
 *
 * @param {docRef} docRef - DocRef to be Deleted
 * @returns {Task} - fork (rej(err),res())
 */
export const deleteDoc = docRef =>
  new Task((rej, res) => docRef.delete().then(res, rej));