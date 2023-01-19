/**
 * Data model for loading the work hour categories and fileld hours.
 * The model implements internal _cache, so the data does not have to be
 * loaded every time from the REST API.
 */
class DataModel {
    /**
     * Initialize the data model with given URL pointing to REST API.
     * @param {string} apiUrl Api URL prefix (without the query part).
     */
    constructor(apiUrl) {
        this.url = apiUrl;
    }
    _cache = null;

    /**
     * Retrieve the data and pass them to given callback function.
     * If the data are available in _cache, the callback is invoked immediately (synchronously).
     * Otherwise the data are loaded from the REST API and _cached internally.
     * @param {Function} callback Function which is called back once the data become available.
     *                     The callback receives the data (as array of objects, where each object
     *                     holds `id`, `caption`, and `hours` properties).
     *                     If the fetch dfailed, the callback is invoked with two arguments,
     *                     first one (data) is null, the second one is error message
     */
    async getData(callback) {
        if (this._cache !== null) return callback(this._cache);
        try {
            const data = await fetch(this.url, { method: "GET" })
                .then((response) => response.json())
                .then((data) => {
                    if (data.ok) return data.payload;
                    else throw data.error;
                });

            await Promise.all(
                data.map((record) =>
                    fetch(`${this.url}?action=hours&id=${record.id}`, { method: "GET" })
                        .then((response) => response.json())
                        .then((data) => {
                            if (data.ok) return data.payload;
                            else throw data.error;
                        })
                        .then((data) => (record.hours = data.hours))
                )
            );

            this._cache = data;
            return callback(data);
        } catch (error) {
            return callback(null, error);
        }
    }

    /**
     * Invalidate internal _cache. Next invocation of getData() will be forced to load data from the server.
     */
    invalidate() {
        this._cache = null;
    }

    /**
     * Modify hours for one record.
     * @param {number} id ID of the record in question.
     * @param {number} hours New value of the hours (m)
     * @param {Function} callback Invoked when the operation is completed.
     *                            On failutre, one argument with error message is passed to the callback.
     */
    setHours(id, hours, callback = null) {
        fetch(`${this.url}?action=hours&id=${id}&hours=${hours}`, { method: "POST" }).then((response) => {
            if (response?.status !== 200) return callback("Bad response from server");

            response
                .json()
                .then((data) => {
                    if (typeof data !== "object") return callback();
                    if (!data.ok) return callback(data.error);
                    if (!this._cache) return callback();

                    const _id = this._cache.findIndex((record) => record.id === id);

                    if (_id != -1) {
                        this._cache[_id].hours = hours;
                    }
                    return callback();
                })
                .catch((err) => callback(err));
        });
    }
}

// In nodejs, this is the way how export is performed.
// In browser, module has to be a global varibale object.
module.exports = { DataModel };
