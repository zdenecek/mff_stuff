
using System;
using System.Collections.Concurrent;
using System.Threading.Tasks;

namespace dns_netcore
{
    class RecursiveResolver : IRecursiveResolver
    {

        private readonly IDNSClient dnsClient;

        private readonly ConcurrentDictionary<string, Task<IP4Addr>> _cache = new();

        private readonly Random random = new Random();
        public RecursiveResolver(IDNSClient client)
        {
            this.dnsClient = client;
        }

        public async Task<IP4Addr> ResolveRecursive(string domain)
        {
            return await ResolveRecursiveInternal(domain);
        }

        private IP4Addr getRootServer()
        {
            // use random server to 
            // avoid concurrent requests to the same server
            // also avoid accidentally selecting only the slowest server
            return dnsClient.GetRootServers()[random.Next(dnsClient.GetRootServers().Count)];
        }

        private async Task<IP4Addr> ResolveRecursiveInternal(string domain, bool noCache = false)
        {

            if (!noCache && _cache.TryGetValue(domain, out var lastTask))
            {

                if (!lastTask.IsCompleted)  // if the task is not completed, return the result
                    return await lastTask;

                // if the task is completed, validate the result
                var resolveFromCache = async () =>
                {
                    try
                    {
                        var reversed = await dnsClient.Reverse(lastTask.Result);
                        if (reversed == domain)
                        {
                            return lastTask.Result;
                        }
                    }
                    catch (DNSClientException)
                    {
                    }
                    // if the reverse fails
                    return await ResolveRecursiveInternal(domain, noCache: true);
                };
                var cacheValidationTask = resolveFromCache();

                _cache[domain] = cacheValidationTask; // we might be discarding updated cache, but it's ok in the long run
                return await cacheValidationTask;
            }

            // Now we are sure we will need to do a resolve unless cache changes.

            // split by first dot 
            int dotIndex = domain.IndexOf('.');
            var isTLD = dotIndex == -1;
            if (isTLD)
            {
                // For single-level domains, resolve using root server
                var rootServer = getRootServer();

                var tldTask = dnsClient.Resolve(rootServer, domain);
                _cache[domain] = tldTask; // we might be discarding updated cache, but it's ok in the long run
                return await tldTask;
            }

            var subdomain = domain.Substring(0, dotIndex);
            var rest = domain.Substring(dotIndex + 1);

            var resolve = async () =>
            {
                var parentTask = ResolveRecursive(rest);
                var parentServer = await parentTask;
                return await dnsClient.Resolve(parentServer, subdomain);
            };
            var task = resolve();
            _cache[domain] = task; // we might be discarding updated cache, but it's ok in the long run
            return await task;
        }
    }

}